#!/bin/bash

adb remount
adb push system/product/app /system/product
adb push system/product/lib /system/product
adb reboot
