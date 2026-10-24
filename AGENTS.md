# ComprehensiveGS 协作规则

## 项目定位

- 本项目是学习 Unreal Engine 动画系统与 ALS 的参考、复刻工程。
- `Plugins/ALS-Refactored` 是上游参考实现（ALS Refactored 4.18，UE 5.8）。除非任务明确要求，否则不要修改插件中的源码或资产。
- 当前复刻代码位于 `Source/ComprehensiveGS`，相关资产位于 `Content`。排查问题时先分清“ALS 原版行为”和“本项目的 `Lls*` 实现”。
- 工程采用渐进式学习/复刻，不要求一次迁移 ALS 的全部功能。

## 每轮开始时的记忆读取

继续处理本项目时，先读取：

1. `.study/README.md`
2. `.study/PROJECT_STATE.md`
3. `.study/DECISIONS.md`
4. `.study/OPEN_QUESTIONS.md`
5. `.study/sessions/` 下最近一次或与当前主题相关的记录

`.study` 记录是工作记忆，不是当前工程状态的绝对真相。若记录与源码、资产、构建结果或用户当前指令冲突，以最新事实和用户当前指令为准，并同步修正记录。

## 讨论结果持久化

- 在每个有实质结果的对话轮次结束时，将结论追加到 `.study/sessions/YYYY-MM-DD.md`。同日多次讨论追加到同一文件，不覆盖旧内容。
- 需要记录的是目标、事实、解释、决定、改动、验证结果、未解决问题和下一步；不保存无信息量的寒暄，也不要求逐字转录。
- 日期与时间使用 `Asia/Shanghai` 时区。
- 工程结构、完成度、已知缺口或当前分支状态发生变化时，更新 `.study/PROJECT_STATE.md`。
- 形成长期有效且不应反复摇摆的选择时，追加到 `.study/DECISIONS.md`。
- 产生尚未决断、需要后续确认的问题时，更新 `.study/OPEN_QUESTIONS.md`。
- 用户明确改变先前决定时，不要静默改写历史；追加新决定并标记它取代了哪一条旧决定。

## 复刻与改动原则

- 优先理解 ALS 的数据流：
  `Character/CharacterMovement -> Character state -> AnimInstance game-thread snapshot -> worker-thread calculations -> Animation Blueprint/Control Rig`。
- 保持现有命名体系：游戏模块类型使用 `Lls` 前缀，资产使用 `ABP_Lls`、`BS_Lls`、`DA_Lls` 等前缀。
- Gameplay Tag 使用 `Lls.*` 命名空间。不要在已有 `Lls` 复刻代码中混入 `Als` 类型或 Tag。
- 修改普通文本源码使用补丁方式；修改 `.uasset`、`.umap` 等二进制资产时优先通过 Unreal Editor/MCP 完成。
- 不覆盖、不撤销用户的未提交资产或源码改动。工作区存在未提交改动时，先识别其范围再操作。
- 当前未提交的 `Content/Character/AnimationInstances/Stances/ABP_Lls_Standing.uasset` 属于用户成果，除非用户明确要求继续编辑，否则保持原样。

## 验证与记录

- C++ 行为改动后至少执行编辑器目标编译；涉及动画资产、状态机或运行时行为时，还应通过 PIE/日志验证。
- 在 `.study/sessions/` 中区分“已通过工具验证”和“仅由源码推得、尚待运行验证”的结论。
- 不把 `Saved/`、`Intermediate/`、`DerivedDataCache/` 的日志全文复制进 `.study`；只摘录与结论有关的证据。

