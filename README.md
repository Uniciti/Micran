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

Запускаем сервер расчитанный на два клиента, а так же эти клиенты.<br>
Аргументы запуска:
1. Режим;
2. Кол-во клиентов;
3. Кол-во потоков;
4. Размер пакета;
5. Кол-во пакетов в секунду;
6. Verbose режим

```bash
./prog server 2 2 16 1 1
./prog client 2 2 16 1 1
./prog client 2 2 16 1 1
```

## Версия компилятора:

g++ (Ubuntu 13.2.0-4ubuntu3) 13.2.0

## Колонка оправданий:

Данная реализация крайне ущербна и забагована, так как ощущается крайняя нехватка как времени, так и знаний.<br> Например, существует баг, при котором несколько клиентских потоков отправляют сообщение одновременно, из-за чего сервер читает их как одно большое. Очевидно, что в данном случае потоки надо синхронизировать.<br>
Также существует баг, при котором у сервера не получается принять входящие соединения. Данную проблему уже исследую, однако времени, явно, не хватит.<br>
P.S. Данный поток мыслей является криком души автора и для прочтения не обязателен.

