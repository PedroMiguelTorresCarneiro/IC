# IC - Information and Coding (2024/25)

---
# PROJECT 1 : Part I

***WORKING README ...***

- **Installing ICU**
```md
#macOs:
    - brew install icu4c
#Linux:
    - sudo apt install libicu-dev
#Windows:
    1. Install vcpkg:
        - git clone https://github.com/microsoft/vcpkg.git
        - cd vcpkg
        - ./bootstrap-vcpkg.bat
    2. Install ICU:
        - vcpkg install icu
```

- **Build and Run**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ./partI
    ```