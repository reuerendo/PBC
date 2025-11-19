# Используем Ubuntu 20.04 как базу
FROM ubuntu:20.04

# Отключаем интерактивные вопросы при установке
ENV DEBIAN_FRONTEND=noninteractive

# 1. Установка зависимостей
# ca-certificates нужен для git clone через https
# 32-битные библиотеки нужны для работы компилятора
RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y \
    git \
    make \
    libc6:i386 \
    libncurses5:i386 \
    libstdc++6:i386 \
    ca-certificates \
    && apt-get clean

# 2. Скачивание SDK через GIT (официальный репозиторий)
WORKDIR /opt
# Используем --depth 1, чтобы не качать всю историю изменений (экономим время и место)
RUN git clone --depth 1 https://github.com/pocketbook/SDK_6.3.0.git

# 3. Настройка путей
# Добавляем папку с компилятором в системный PATH
ENV PATH="/opt/SDK_6.3.0/bin:${PATH}"

# Рабочая директория
WORKDIR /app
