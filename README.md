# Тестовое задание для "Цитадель"

## Скачивание репозитория

### 1. Клонируйте репозиторий
   ```bash
   git clone https://github.com/Gr0miLa/citadel-test-case.git
   ```

## Настройка докера в VS Code

### 1. Откройте папку проекта в VS Code
### 2. `F1` → `Dev Containers: Reopen in Container`

## Настройка докера вручную

### 1. Сборка образа
   ```bash
   docker build -t cpp-dev -f .devcontainer/Dockerfile .
   ```

### 2. Запуск контейнера
   ```bash
   docker run -it --rm -v $(pwd):/workspace -w /workspace cpp-dev bash
   ```

## Сборка проекта

### 1. Соберите проект
   ```bash
   mkdir build && cd build
   cmake .. && make
   ```

### 2. Использование парсера
Файлы для теста хранятся в директории reports, поэтому после сборки 
используйте команду (из папки build)
   ```bash
    ./parser ../reports/*.txt
   ```

Получаем следующий вывод:
```
sensor1:
    temp: max=42(file2.txt), min=36.6(file1.txt)
    speed: max=1.4 Gbit/s(file2.txt), min=765 Mbit/s(file1.txt)

sensor2:
    state: max=включен(file2.txt), min=выключен(file1.txt)
    speed: max=1.5 Kbit/s(file1.txt), min=0 bit/s(file2.txt)
```

## Структура проекта
```
citadel-test-case/
├── .devcontainer/            # Конфигурация контейнера
│   ├── devcontainer.json     # Настройки VS Code
│   └── Dockerfile            # Образ для разработки
├── config/                   # Директория для конфига
│   └── config.json           # Конфиг
├── include/                  # Заголовочные файлы
│   ├── Config.hpp            # Заголовочный файл для работы с конфигом
│   ├── FileReader.hpp        # Заголовочный файл FileReader
│   └── Parser.hpp            # Заголовочный файл Parser
├── reports/                  # Директория с данными
│   ├── file1.txt             # Первый файл с данными
│   └── file2.txt             # Второй файл с данными
├── src/                      # Исходный код
│   ├── CMakeLists.txt        # Конфигурация CMake для src
│   ├── Config.cpp            # Реализация методов для работы с конфигом
│   ├── FileReader.cpp        # Реализация FileReader
│   ├── main.cpp              # main
│   └── Parser.cpp            # Реализация Parser
├── third_party/              # Сторонние библиотеки
│   └── json.hpp              # Header-only библиотека nlohmann/json
├── .gitignore                # Список файлов и папок, игнорируемых Git
├── CMakeLists.txt            # Корневой CMake
└── README.md                 # Ридмишка
```
