# raylib-starter

## Building Raylib

First build raylib. Go to lib/raylib/src/ and run

```
make PLATFORM=PLATFORM_%%%
```

depending on your preferred platform.

## Building your project

Navigate to the base directory and simply compile using

```
make -f desktop.mk
```

for desktop

or

```

make -f web.mk

```

and run using

```
emrun build/index.html


```

