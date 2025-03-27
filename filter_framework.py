import pandas as pd
import ta  # For technical indicators (you might already have this)
import logging
import matplotlib.pyplot as plt

# ----------------------
# Logging Configuration (as in your script)
# ----------------------
log_file = "filter_testing.log"  # Separate log file
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[logging.FileHandler(log_file, mode='w'), logging.StreamHandler()]
)
logger = logging.getLogger(__name__)

# ----------------------
# 1. Modular Filter Implementation
# ----------------------

class TradingFilter:
    """Base class/interface for trading filters."""
    def apply(self, data: pd.DataFrame) -> pd.Series:
        raise NotImplementedError("Subclasses must implement the apply method.")

def rsi_filter(data: pd.DataFrame, rsi_period=14, rsi_oversold=30, rsi_overbought=70) -> pd.Series:
    """RSI-based filter."""
    rsi = ta.rsi(data['Close'], window=rsi_period)
    return (rsi > rsi_oversold) & (rsi < rsi_overbought)

def macd_filter(data: pd.DataFrame, fast_period=12, slow_period=26, signal_period=9) -> pd.Series:
    """MACD-based filter."""
    macd = ta.macd(data['Close'], window_fast=fast_period, window_slow=slow_period, window_sign=signal_period)
    return macd['macd'] > macd['macd_signal']

def bollinger_band_filter(data: pd.DataFrame, window=20, num_std_dev=2) -> pd.Series:
    """Bollinger Band filter."""
    bb_upper = ta.bollinger_hband(data['Close'], window=window, nbdev=num_std_dev)
    bb_lower = ta.bollinger_lband(data['Close'], window=window, nbdev=num_std_dev)
    return (data['Close'] > bb_lower) & (data['Close'] < bb_upper)

# Add more filters here...

# ----------------------
# 2. Data Structure for Tracking
# ----------------------

def run_backtest(data: pd.DataFrame, filters: dict, initial_balance=1000):
    """
    Simulates trading and tracks performance for a set of filters.

    Args:
        data: Historical price data (Pandas DataFrame).
        filters: A dictionary of filter functions (name: function).
        initial_balance: Starting balance for the simulation.

    Returns:
        A dictionary containing the results of the backtest for each filter.
    """

    backtest_results = {}
    for filter_name, filter_func in filters.items():
        logger.info(f"Running backtest for filter: {filter_name}")
        filter_signals = filter_func(data)
        trades = []
        balance = initial_balance
        position = 0  # 0: no position, 1: long

        for i in range(len(data)):
            if filter_signals[i] and position == 0:
                # Buy logic (simplified)
                cost = data['Close'][i]
                if balance > cost:
                    position = 1
                    buy_price = cost
                    balance -= cost
                    trades.append({'type': 'buy', 'price': buy_price, 'time': data.index[i]})
                    logger.info(f"BUY at {buy_price} on {data.index[i]}")

            elif not filter_signals[i] and position == 1:
                # Sell logic (simplified)
                sell_price = data['Close'][i]
                profit = sell_price - buy_price
                balance += sell_price
                position = 0
                trades.append({'type': 'sell', 'price': sell_price, 'time': data.index[i], 'profit': profit})
                logger.info(f"SELL at {sell_price} on {data.index[i]}  Profit: {profit}")

        # Calculate performance metrics
        sell_trades = [trade for trade in trades if trade['type'] == 'sell']
        profitable_trades = sum(1 for trade in sell_trades if trade['profit'] > 0)
        loss_trades = sum(1 for trade in sell_trades if trade['profit'] <= 0)
        win_rate = (profitable_trades / len(sell_trades)) * 100 if sell_trades else 0
        total_profit = sum(trade['profit'] for trade in sell_trades)

        backtest_results[filter_name] = {
            'final_balance': balance,
            'num_trades': len(sell_trades),
            'win_rate': win_rate,
            'total_profit': total_profit,
            # Add more metrics as needed
        }
        logger.info(f"Backtest results for {filter_name}: {backtest_results[filter_name]}")
    return backtest_results

# ----------------------
# 4. Evaluation and Comparison
# ----------------------

def evaluate_and_compare(results: dict):
    """
    Evaluates and compares the backtest results of different filters.

    Args:
        results: A dictionary of backtest results (from run_backtest).
    """
    logger.info("--- Filter Evaluation ---")
    for filter_name, metrics in results.items():
        logger.info(f"Filter: {filter_name}")
        for metric, value in metrics.items():
            logger.info(f"  {metric}: {value}")

    # Simple ranking by final balance (you can expand this)
    best_filter = max(results, key=lambda k: results[k]['final_balance'])
    logger.info(f"Best performing filter: {best_filter} with final balance: {results[best_filter]['final_balance']}")

    # --- Visualization Example (requires matplotlib) ---
    filter_names = list(results.keys())
    final_balances = [results[name]['final_balance'] for name in filter_names]

    plt.bar(filter_names, final_balances)
    plt.xlabel('Filters')
    plt.ylabel('Final Balance')
    plt.title('Comparison of Filter Performance')
    plt.show()

if __name__ == "__main__":
    # --- Example Usage ---

    # Generate some dummy data (replace with your actual data)
    data = pd.DataFrame({
        'Close': [100, 102, 105, 103, 106, 108, 105, 110, 112, 115, 110, 108, 105, 102, 100, 98, 95, 97, 100, 103]
    })
    data.index = pd.to_datetime(range(len(data)), unit='D', origin='2025-01-01')

    filters_to_test = {
        'RSI Filter': rsi_filter,
        'MACD Filter': macd_filter,
        'Bollinger Band Filter': bollinger_band_filter,
        # Add more filters here
    }

    results = run_backtest(data, filters_to_test)
    evaluate_and_compare(results)
