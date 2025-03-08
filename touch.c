#include "touch.h"
#include "lcd.h"

#include <linux/input.h> 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

int min_x = 0x7FFFFFFF, max_x = 0;
int min_y = 0x7FFFFFFF, max_y = 0;

int* get_touch()
{
    int fd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1) {
        perror("touch faild");
        return NULL;
    }

    int x = 0, y = 0;
    int touch_pressed = 0;
    struct input_event event;

    while (1) {
        size_t reader = read(fd, &event, sizeof(event));
        if (reader < sizeof(event)) {
            continue;
        }

        switch (event.type) {
            case EV_ABS:
                switch (event.code) {
                    case ABS_X:
                        x = event.value;
                        if (min_x > x) min_x = x;
                        if (max_x < x) max_x = x;
                        break;
                    case ABS_Y:
                        y = event.value;
                        if (min_y > y) min_y = y;
                        if (max_y < y) max_y = y;
                        break;
                    case ABS_PRESSURE:
                        if (event.value > 0) {
                            touch_pressed = 1;
                        } else {
                            touch_pressed = 0;
                        }
                        break;
                }
                break; // 增加 break 防止穿透
            case EV_KEY:
                if (event.code == BTN_TOUCH) {
                    if (event.value == 1) {
                        touch_pressed = 1;
                    } else {
                        // 触摸释放时返回坐标
                        if (touch_pressed) {
                            int* arr = (int*)malloc(2 * sizeof(int));
                            if (arr == NULL) {
                                perror("malloc failed");
                                close(fd);
                                return NULL;
                            }
                            arr[0] = x;
                            arr[1] = y;
                            close(fd);
                            return arr;
                        }
                        touch_pressed = 0;
                    }
                }
                break;
        }

        if (touch_pressed) {
            printf("pressing:x:%d   y:%d\n", x, y);
        }
    }

    close(fd);
    return NULL;
}


int get_direction()
{
    int fd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open touchscreen device");
        return -1;
    }
    int x0 = -1, y0 = -1;
    int x1 = -1, y1 = -1;
    int touch_pressed = 0;
    struct input_event event;
    while (1) {
        size_t bytes = read(fd, &event, sizeof(event));
        if (bytes == -1) {
            perror("Failed to read touch event");
            break;
        }
        if (event.type == EV_ABS) {
            if (event.code == ABS_X) {
                if (x0 == -1) {
                    x0 = event.value;
                }
                x1 = event.value;
            } else if (event.code == ABS_Y) {
                if (y0 == -1) {
                    y0 = event.value;
                }
                y1 = event.value;
            } else if (event.code == ABS_PRESSURE) {
                if (event.value > 0) {
                    touch_pressed = 1;
                } else {
                    if (touch_pressed) {
                        break;
                    }
                    touch_pressed = 0;
                }
            }
        } else if (event.type == EV_KEY) {
            if (event.code == BTN_TOUCH) {
                if (event.value == 1) {
                    touch_pressed = 1;
                } else {
                    if (touch_pressed) {
                        break;
                    }
                    touch_pressed = 0;
                }
            }
        }
    }
    close(fd);
    int xx = abs(x1 - x0);
    int yy = abs(y1 - y0);
    if (xx > yy) {
        if (x1 > x0) {
			printf("right\n");
            return 1;
        } else {
            printf("left\n");
			return 2;
        }
    } else {
        if (y1 > y0) {
            printf("down\n");
			return 3;
        } else {
            printf("up\n");
			return 4;
        }
    }
    return 0;
}
