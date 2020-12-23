# Chinese-calendar-uk

**It is the default calendar for Ubuntu Kylin.**

**But it can be used in other Linux desktop environment (such as KDE).**

"Chinese calendar" is a Chinese traditional calendar for Ubuntu Kylin. You can  find festivals and solar terms for China. In line with the Chinese using habit, it can help users to query date easily.

> This is a personal fork from Launchpad to GitHub.
>
> It has been unmaintained since 2017. So I uploaded its codes to GitHub, wishing the developers who care of Chinese lunar calendar desktop client may feel free to fork it.

## Known Issue(s)

As for now, once the window show up, it will be quickly crashed. (Arch Linux KDE)

## How to build

Firstly, you need `qt5-base` and `qt5-svg` to be installed.

```bash
git clone https://github.com/whriedplanck/Chinese-calendar-uk
cd Chinese-calendar-uk
copy src/datemaskwidget.h ./datamaskwidget.h # I can't figure out how to build without this step.
qmake PREFIX=/usr
make
make install
```

## To Do

- [x] Reorganized the structure of the folders and files.

- [ ] Replace qmake with cmake. (Not sure)
- [ ] Improve UI.

## Screenshots

![1.png](https://www.ubuntukylin.com/public/images/928_nl1.png)

![2.png](https://www.ubuntukylin.com/public/images/928_nl2.png)

![3.png](https://www.ubuntukylin.com/public/images/928_nl3.png)

## License

GPL-3.0 License

