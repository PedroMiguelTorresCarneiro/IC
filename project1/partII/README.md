# IC - Information and Coding (2024/25)

--
# PROJECT 1 : Part II

- **Installing Boost.locale** (to work with UTF-8 encoding)
```bash
#macOs:
    Dowload the files from the SFML website, the cmake file will locate it 
#Linux:
    sudo apt-get update
    sudo apt-get install libsfml-dev
#Windows:
    Download the files directly from the SFML website
```

- **Build and Run**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ./SoundDecoder
    ```

    Or, just:
    
    $ sh run.sh