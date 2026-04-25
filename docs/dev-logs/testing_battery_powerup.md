# Reasons device might not boot on battery

1. The board does not auto-start when you plug in a battery the way it does with USB. You have to press and hold the physical power button until the firmware latches GPIO7 HIGH. A quick tap probably isn't enough - make sure you hold it for a full second.

2. Battery too flat — below ~3V the board won't start. A brand new 1100mAh cell from storage might be low.

3. JST polarity reversed - this would prevent any power at all.. and could be how the LiPo came from factory.

## Most recent code update

With the most recent update, now when you boot on battery, the screen will flash bright 3 times in quick succession. If you see the flashes the board is alive, the battery is working, and the ESP32-S3 (not DualEye) GPIO7 latch succeeded. If nothing one of the three failure modes above.

## Step-by-step for testing battery

1. Flash the firmware over USB first (do this once)
2. Unplug USB
3. Plug in the LiPo
4. Press and hold the power button for about 1 second (NOT a quick tap!)
5. Watch for 3 backlight flashes on the round display

If no flashes on screen: try a longer button hold, or charge the battery via USB first (the charge LED on the board will tell you if it's taking a charge).
