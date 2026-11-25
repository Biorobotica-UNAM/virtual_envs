これは何？
-----------------
マニピュレーション用のメッセージおよび、サービスを定義する．

定義されているmsg一覧
---------------------
- CollisionEnvironment プランナや干渉チェックで使う３次元環境情報
- CollisionObject プリミティブやメッシュで表された既知物体情報
- HandMove ハンドへの指令を表す
- CollisionObjectOperation CollisionObjectの操作 enum
- HandConstraint 手先拘束の種類を表す enum
- ArmManipulationErrorCodes アームのマニピュレーションで起こるエラーコード enum
- HandOperation ハンドの操作を表す enum
- GraspState 把持状態を表す

定義されているsrv一覧
---------------------
- SolveCollisionFreeIK 干渉フリーなIKを解く
- PlanWithJointGoal 与えられた全身関節角までのパスをプランニング
- PlanWithPoseGoal 与えられた手先姿勢までのパスをプランニング
- PlanFollowPath 与えられた直線軌道や円弧軌道のパスをプランニングする
- ValidateJoint 与えられた姿勢が干渉していないかチェック
- ValidateTrjectory 与えられた軌道が干渉していないかチェック
- GetGraspState 把持状態を取得する

メッセージの詳細
--------------------
CollisionEnvironmnet プランニング等でロボットと干渉するものすべてをまとめたメッセージ．既知物体とボクセルマップからなる.
: *header(Header)* 時間と基準フレームの情報が入る．
: *known_objects(tmc_manipulation_msgs/CollisionObject[])* 既知物体のリストが入る．
: *poses(getenvironmentmap[])* 既知物体の基準フレームからみた位置姿勢が入る．既知物体のリストと同じ長さにすること．
: *collision_map(mapping_msgs/CollisionMap)* レーザ等でとった干渉するボクセル情報が入る．
: *collision_map_pose(geometry_msgs/Pose)* collision_mapの基準フレームから見た座標系が入る．


CollisionObject 干渉検出に用いる物体を表すメッセージ．単純形状であるShapeの集合で表される．
: *header(Header)* 時間と基準フレームの情報が入る．
: *id(tmc_msgs/ObjectIdentifier)* 物体の数字のidと名前を合わせたもの．
: *operation(CollisionObjectOperation)* 物体を追加したり消したりするときはここで意味を付加．
: *shapes(tmc_geometric_shapes_msgs/Shape[])* 物体の形状情報のリスト．
: *poses(geometry_msgs/Pose)* shapesのそれぞれの位置姿勢、CollisionObject基準．

HandMove 把持や把持開放の指令
: *hand_id(hand_id)* ハンドのid FSMの配列番号に対応
: *operation(tmc_manipulation_msgs/HandOperation)* ハンドの動作種類。GraspやOpen等
: *grasping_parameter* 把持パラメータ 把持力やスピード等。詳細はハンドのFSM参照。

CollisionObjectOperation 物体の追加 enum．
: *operation(byte)* 操作の種類 現在はADD,REMOVEのみ有効．
: *ADD* 物体追加．
: *REMOVE* 物体削除．

HandConstraint 手先の回転拘束を表す enum．
: *operation(byte)* 手先拘束の種類．
: *kConstraintOff* 拘束なし．
: *kHingeConstraint* ヒンジ（１軸）拘束．
: *kFullConstraint* 全軸拘束．


ArmManipulationErrorCodes アームのエラーコードをまとめたメッセージ enum
: *val(int32)* エラーコードが入る．
: 各種エラー定数ROSの定義を仮にコピーしたもの．

GraspState 把持状態を表すメッセージ
: *header(Header)* 把持認識を行った時刻
: *hand_module_name(string)* ハンドモジュール名（trmlにて定義される名称）．例：`CARM/HAND`
: *state(uint8)* 把持の状態
: *kInvalid* 無効
: *kEmpty* 何も把持していない
: *kUnstable* 動作中のため判断保留
: *kGrasped* 把持している


MultiDOFJointTrjaectoryPoint hydroからtrajectory_msgsに追加されているがgroovyで使えないのでこれを用いる

MultiDOFJointTrajectory hydroからtrajectory_msgsに追加されているがgroovyで使えないのでこれを用いる

サービスの詳細
--------------------
GetCollisionEnvironment 干渉チェックのための環境取得メッセージ．
: **request** *origin_frame_id(string)* 取得する環境のフレーム CollisionEnvironmentはこの座標系基準で取得される．
: **request** *known_object_only(bool)* trueなら物体情報のみでボクセル情報を取得しない．
: **response** *environment(tmc_manipulation_msgs/CollisionEnvironment)* 取得した環境情報. origin_frame_idで指定した基準で取得される．

SetCollisionEnvironment 干渉チェックのための環境設定メッセージ．
: **request** *environment(tmc_manipulation_msgs/CollisionEnvironment)* 設定する環境情報．
: **request**

GetGraspState 把持状態取得のためのサービス
: **request** *hand_module_name(string)* ハンドモジュール名（trmlにて定義される名称）
: **response** *result(tmc_manipulation_msgs/GraspState)* 把持状態

