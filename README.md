# Проект 14 "Fullstack разработка системы управления"

Это многопоточное клиент-серверное приложение на языке программирования C++, которое позволяет передавать и принимать данные по сети с использованием TCP протокола.

## Краткое описание

В программе реализовано два режима: сервера и клиента. Сервер принимает данные от клиентов, а клиенты генерируют и отправляют данные серверу. Клиенты и сервер работают многопоточно.

## Компиляция и запуск

### Компиляция:

```bash
g++ p1.cpp -o prog
```

### Запуск программы

#### Шаблон:

./prog <mode: server/client> <max_clients int> <num_threads int> <packet_size int> <packets_per_second int> <verbose 1/0>

#### Пример запуска:

Запускаем сервер расчитанный на два клиента, а так же эти клиенты.
Аргументы запуска:
1. Кол-во клиентов;
2. Кол-во потоков;
3. Размер пакета;
4. Кол-во пакетов в секунду;
5. Verbose режим

```bash
./prog server 2 2 16 1 1
./prog client 2 2 16 1 1
./prog client 2 2 16 1 1
```

## Версия компилятора:

g++ (Ubuntu 13.2.0-4ubuntu3) 13.2.0
