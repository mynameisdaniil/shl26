---
Что установить для всех:

### Arduino
[Arduino IDE](https://www.arduino.cc/en/software/)
Выбирайте свою платформу и устанавливайте:
![](./img/arduino.png)

### Board support для Arduino
Слева на панели находим кнопку как на картинке (Boards, Менеджер Плат)
![](./img/arduino_rp2040.png)
Нажимаем "УСТАНОВКА"

### Остальные утилитки
для MacOS
```
brew update
brew install make hexyl arm-none-eabi-gcc stlink open-ocd screen
```
или для Ubuntu
```
sudo apt update
sudo apt install  make hexyl gcc-arm-none-eabi stlink-tools openocd screen
```
или для Rocky / Alma / RHEL
```
sudo dnf install epel-release

sudo dnf install make hexyl arm-none-eabi-gcc-cs arm-none-eabi-binutils-cs arm-none-eabi-newlib stlink openocd screen
```
или для openSUSE
```
sudo zypper install make hexyl cross-arm-none-eabi-gcc stlink openocd screen
```

