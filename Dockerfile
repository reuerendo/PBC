# Используем старую Ubuntu, так как SDK требует старых библиотек
FROM ubuntu:20.04

# Установка необходимых инструментов и 32-битных библиотек (SDK 32-битный)
RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y wget make git libc6:i386 libncurses5:i386 libstdc++6:i386 && \
    apt-get clean

# Скачивание и распаковка PocketBook SDK 6.3.0
WORKDIR /opt
# Используем официальное зеркало PocketBook
RUN wget -q -O sdk.tar.gz http://support.pocketbook-int.com/SDK/PocketBook_SDK_6.3.0.tar.gz && \
    tar -xzf sdk.tar.gz && \
    rm sdk.tar.gz

# Добавляем компилятор в PATH
ENV PATH="/opt/PocketBook_SDK_6.3.0/bin:${PATH}"

# Рабочая директория для нашего приложения
WORKDIR /app