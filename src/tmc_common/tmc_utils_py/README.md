`tmc_utils_py`
===============================================================================

概要
-------------------------------------------------------------------------------
トピックや幾何計算に便利なPythonの関数・クラスを提供する


クラス
-------------------------------------------------------------------------------
### tmc_utils_py.caching_subscriber.CachingSubscriber
コンストラクタ引数で指定した任意のトピックを購読し，保持するクラス．

### tmc_utils_py.switch_monitor.LongPressedMonitor
True/Falseが定期的に入力されると，一定期間Trueが続くかを判断するクラス．

### tmc_utils_py.switch_monitor.RepeatedPressMonitor
True/Falseが定期的に入力されると，False->Trueの遷移が短期間に連続したかを判断するクラス．

### tmc_utils_py.value_state_monitor.ValueStateMonitor
入力値に特定の関数を適用したものを状態とし、状態が変化したか判定を行うクラス.