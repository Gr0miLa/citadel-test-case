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
используйте команду 
   ```bash
    ./parser ../reports/*.txt
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
