# 工程状态快照

- 快照日期：2026-10-24
- 分支：`main`
- 最新提交：`299b804 left to right hip switch.`
- 引擎版本：Unreal Engine 5.8
- ALS 参考版本：ALS Refactored 4.18，位于 `Plugins/ALS-Refactored`

## 总体结构

| 区域 | 作用 |
| --- | --- |
| `Source/ComprehensiveGS` | 当前学习复刻的主运行时模块 |
| `Content/Character` | 角色蓝图、主/站立动画蓝图、Overlay 动画层接口 |
| `Content/Animation/Grounded/WalkRun` | 六个方向的 WalkRun 混合空间 |
| `Content/Data/Character` | 角色、移动、动画实例 Data Asset |
| `Content/Input` | Enhanced Input 的 Move、Sprint 与默认映射上下文 |
| `Plugins/ALS-Refactored` | 可直接运行、用于对照的 ALS 原版参考 |
| `.study` | 学习理解、决定和跨会话记忆 |

## C++ 数据流

当前代码基本沿用了 ALS 的分层思路，但规模明显缩小：

1. `ALlsCharacter` 负责输入入口之外的角色状态、Gait、旋转模式、地面旋转和目标朝向。
2. `ULlsCharacterMovementComponent` 保存 Gait 设置和最大允许 Gait，并负责切换最大移动速度。
3. `ULlsAnimationInstance` 在游戏线程复制角色状态，在工作线程刷新 Pose、Standing、Grounded、MovementDirection、VelocityBlend 与 Feet 动画数据。
4. `FLlsAnimationInstanceProxy::PostUpdate` 回写动画实例，模拟 ALS 对 `NativePostUpdateAnimation` 的扩展方式。
5. `ULlsLinkedAnimationInstance` 为站立等 Linked Anim Layer 提供父动画实例访问。

核心源码：

- `Source/ComprehensiveGS/Private/LlsCharacter.cpp`
- `Source/ComprehensiveGS/Private/LlsCharacterMovementComponent.cpp`
- `Source/ComprehensiveGS/Private/LlsAnimationInstance.cpp`
- `Source/ComprehensiveGS/Public/State/`
- `Source/ComprehensiveGS/Public/Settings/`
- `Source/ComprehensiveGS/Public/Utility/`

## 动画曲线数据路径

`ULlsAnimationInstance::RefreshFeet()` 中的 `GetCurveValue("FeetCrossing")` 读取的是当前动画求值结果里的 `AttributeCurve`，不是角色运行时根据脚骨骼位置计算出的数据，也不是 `FLlsGroundedSettings` 中保存的 `UCurveFloat` 资产。

数据路径如下：

`AnimSequence 内部 Curve 数据 -> Blend Space/动画图混合 -> 最终 FAnimInstanceProxy::AnimationCurves -> GetCurveValue -> FeetState.FeetCrossingAmount -> 动画图/Control Rig`

本项目当前六个 `BS_Lls_WalkRun_*` 混合空间仍直接引用 ALS 插件中的 `A_Als_*` 动画序列。例如 `BS_Lls_WalkRun_Left_Forward` 依赖 `A_Als_Walk_Left_Forward`，而该序列的资产标签 `CurveNameList` 包含 `FeetCrossing`。因此项目 Content 中虽然没有单独的 FeetCrossing 曲线资产，运行时仍可从 ALS 序列获得该曲线。

`FeetCrossingAmount` 的语义是“当前播放/混合到的侧向移动动画处于双脚交叉阶段的归一化程度”，取值范围为 0 到 1。它不是两只脚在空间中的实际距离，也不表示哪只脚着地。该信息用于动画图在侧移、斜向移动和方向切换时理解摆动腿与支撑腿的姿态阶段，避免髋部方向切换、侧移循环混合和脚部处理产生不自然的交叉或跳变。

ALS 的 Standing/Crouching 动画蓝图会通过 Property Access 读取 `FeetState.FeetCrossingAmount`，并在移动状态中与 `GroundedState.HipsDirectionLockAmount` 等数据共同使用。C++ 层只采集并公开这条曲线，具体动画图决策不在 `RefreshFeet()` 内。`FootPlantedAmount` 是有符号的落脚/抬脚参数，而 `FeetCrossingAmount` 是无符号的时刻/程度参数，两者职责不同。

`HipsDirectionLockAmount` 是从 `HipsDirectionLock` 曲线读取、限制在 `[-1, 1]` 的髋部方向锁定信号。它不是骨盆旋转角度，也不是关节或 Foot Lock 权重，而是给动画状态机选择 `HipsDirection` 时使用的有符号偏置：0 表示不覆盖正常的髋部方向选择，正负值表示把髋部方向锁向两个镜像方向之一，绝对值表示锁定强度。其最终用途是让基础移动姿态与 Overlay/瞄准姿态所要求的站立腿关系保持一致，避免切换 Overlay 时下半身突然左右换脚或产生不自然的腿部交叉。

在 ALS 内容中，`HipsDirectionLock` 主要出现在 Bow、Rifle、Pistol、Torch 等 Overlay Poses 序列，普通 WalkRun 循环和 `A_Als_Default_Poses` 不包含它。实际键值以 `-1/0/+1` 的分段信号为主；例如 Bow Poses 中出现 `+1` 区间，PistolOneHanded Poses 中出现 `-1` 区间。Standing/Crouching 动画蓝图通过 Property Access 读取 `GroundedState.HipsDirectionLockAmount`，并把它与 `FeetCrossingAmount` 配合使用。当前项目默认 Overlay 依赖不包含该曲线的 `A_Als_Default_Poses`，所以默认状态下该值通常为 0。

## 已完成或已存在的临时成果

### 角色与移动

- 创建 `ALlsCharacter`，用自定义 `ULlsCharacterMovementComponent` 替换默认 CharacterMovement。
- Enhanced Input 已接入 WASD Move 和 Sprint。
- 已实现 Desired Gait、Max Allowed Gait、实际 Gait 的基础计算。
- 已实现地面 ViewDirection / VelocityDirection 的基础旋转目标与阻尼旋转。
- 已包含移动平台的基础 MovementBase 状态刷新。

### 动画实例

- 已建立角色状态到动画状态的游戏线程快照。
- 已实现 Pose 曲线、Standing Stride/PlayRate、Grounded MovementDirection、VelocityBlend、基础 Feet 曲线读取。
- 已实现线程安全与 Game Thread 更新的职责分层。

### 内容资产

- 主动画蓝图：`Content/Character/ABP_Lls.uasset`
- 站立 Linked Animation Blueprint：`Content/Character/AnimationInstances/Stances/ABP_Lls_Standing.uasset`
- Overlay 层接口：`Content/Character/ALI_Lls_Overlay.uasset`
- 默认 Overlay 层：`Content/Character/AnimationInstances/Overlays/ABP_Lls_Overlay_Default.uasset`
- 六个方向 WalkRun Blend Space：Forward、Backward、LeftForward、LeftBackward、RightForward、RightBackward。
- `ABP_Lls_Standing` 当前包含 Idle/Move 主状态机、六方向 Movement States 和 Movement Details/Run 状态。

## 当前未提交成果

截至 2026-10-24，`Content/Character/AnimationInstances/Stances/ABP_Lls_Standing.uasset` 和 `Source/ComprehensiveGS/Public/State/LlsFeetState.h` 存在未提交改动。前者与 `HEAD` 相比，LFS 记录的资产大小由 502,921 字节变为 659,898 字节；后者新增了一条解释 `FeetCrossingAmount` 为 1 时表示双脚交叉的注释。这些改动属于用户当前成果，视为受保护的工作区状态。

## 已确认的缺口与风险

以下为源码静态检查结果，除特别说明外尚未逐项做运行时回归：

- `RefreshGroundedMovement` 已读取 `HipsDirectionLockAmount`，但还没有计算 `GroundedState.HipsDirection`。
- `RefreshFeet` 目前只读取 `FootPlantedAmount`、`FeetCrossingAmount` 曲线，尚未实现真正的 Foot Lock、脚部 IK 和目标脚部变换。
- `RefreshLocomotionLate` 为空；空中、落地、蹲伏、瞄准、滚翻、翻越、Montage Action 等 ALS 子系统尚未迁入。
- `CanSprint` 仍是占位实现并返回 `false`，因此 Sprint 请求会被限制为 Running。
- `ULlsCharacterMovementComponent::GaitAmount` 尚无刷新计算；当前角色旋转速度曲线因此使用默认 HalfLife。
- `MovementSettings` 虽已有 Data Asset，但 BP 的 CharacterMovement 上尚未看到赋值，运行时应检查是否仍使用结构默认值。
- 简化版 `RefreshView` 未像 ALS 那样更新 ViewState 的最终旋转；`YawSpeed` 很可能长期为 0，需要 PIE 验证。
- `LlsAnimationInstance.cpp` 中 MovementDirection 每个动画更新都有多条 `UE_LOG(Display)`，属于临时调试输出，后续应清理或改为受控调试开关。

## 环境日志说明

最近编辑器日志中存在 GameFeatureData 未加入 Asset Manager 扫描规则的错误，以及 ALS 内容“empty engine version”的加载警告。这些没有阻止编辑器载入项目，但不能据此宣称项目完全无错误；涉及相关功能时再单独验证。

## 建议的学习推进顺序

这不是已批准路线，只是基于当前缺口的推荐顺序：

1. 收尾并验证当前左右髋部方向切换。
2. 补齐 Movement Settings、GaitAmount 和速度/旋转曲线。
3. 实现 InAir 状态与 Jump/Fall/Land 动画切换。
4. 实现 Foot Lock、Foot IK 与斜坡/台阶适配。
5. 实现 Rotate In Place 与 Pivot。
6. 扩展 Linked Anim Layers、Overlay 运行时切换和 Montage Action。
7. 最后处理网络同步、调试可视化和更完整的 ALS 功能。

