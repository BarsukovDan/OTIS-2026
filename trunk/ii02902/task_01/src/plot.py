import sys
sys.path.append(r"D:\lib\site-packages")
import os
import pandas as pd
import matplotlib.pyplot as plt

# Путь к файлу с результатами (лежит в корне проекта)
csv_path = "results.csv"
if not os.path.exists(csv_path):
    print("Файл results.csv не найден! Сначала запустите программу симуляции.")
    exit(1)

# Загружаем данные
df = pd.read_csv(csv_path)

plt.figure(figsize=(10, 5))

# Создаем основную ось для выхода y
fig, ax1 = plt.subplots(figsize=(10, 5))

model_title = df['model_name'].iloc[0] if 'model_name' in df.columns else "Model"
signal_title = df['signal'].iloc[0] if 'signal' in df.columns else "Signal"

ax1.set_title(f"Моделирование: {model_title} ({signal_title})")
ax1.set_xlabel("Шаг (step)")
ax1.set_ylabel("Выход y", color='b')
line1 = ax1.plot(df['step'], df['y'], marker='o', color='b', label='Output y')
ax1.tick_params(axis='y', labelcolor='b')
ax1.grid(True)

# Создаем вторую ось (twinx) для входного сигнала u, чтобы они не перекрывали друг друга по масштабу
ax2 = ax1.twinx()
ax2.set_ylabel("Вход u", color='r')
line2 = ax2.plot(df['step'], df['u'], marker='x', color='r', linestyle='--', label='Input u')
ax2.tick_params(axis='y', labelcolor='r')

# Объединяем легенды с обеих осей в одно окошко
lines = line1 + line2
labels = [l.get_label() for l in lines]
ax1.legend(lines, labels, loc='upper left')

plt.tight_layout()

# Сохраняем графики в папку doc и в корень проекта для отчёта (используем имя plot_task01.png, как в README)
os.makedirs("doc", exist_ok=True)
plt.savefig("doc/plot.png")
plt.savefig("plot_task01.png")

# Показываем окно с графиком
plt.show()  