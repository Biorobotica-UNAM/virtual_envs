これは何？
----------
アームプランニングのためのmsgとsrv．

関係者
------
寺田耕志

提供するメッセージ
-----------------

* AttachedObject: プランニングのリクエストの際に物体を特定部位に取り付けるためのメッセージ．

* Constraints: jointとlinkの拘束条件を格納するメッセージ.

* JointPosition: 関節の角度のみを表すためのメッセージ．

* RangeJointConstraint: jointの拘束条件を上下限の範囲で表すためのメッセージ.

* RobotLocalGoal: RobotLocalPlannerの実行要求を発行するためのメッセージ.

* TaskSpaceRegion: プランニングの際の拘束および，ゴールやスタートを領域として表す．

* TsrLinkConstraint: TSRを用いたlinkの拘束条件を表すためのメッセージ.


提供するサービス
---------------

* PlanWithJointGolas: 関節角度をゴールとしたプランニング

* PlanWithHandGolas: 手先位置をゴールとしたプランニング

* PlanWithHandLine: 手先を直線に添うようにプランニング

* PlanWithTsrConstraints: TSRを使ったプランニング．上級者向けで機能が豊富．


提供するアクション
----------------

* EvaluateRobotTrajectories: 軌道評価アクション

* GenerateRobotTrajectories: 軌道生成アクション

* OptimizeRobotTrajectory: 軌道最適化アクション

* ValidateRobotTrajectories: 軌道干渉チェックアクション
