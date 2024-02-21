# KL05Z-RTC-alarm
Project Description
The alarm clock program with the function of entering the current time, setting the alarm, and snoozing was developed for the FRDM-KL05Z board. Timekeeping was implemented using the internal RTC module, which tracks the current time using a quartz clock. The alarm function was realized using the RTC ALARM interrupt, which triggers when the alarm time matches the current time. Time input was implemented using a button matrix and GPIO interrupts. Displaying input data and the current time was done using a popular two-line, 16-character alphanumeric LCD display with an HD44780U controller, which communicates with the board via the I2C serial communication bus. Alarm signaling utilizes an LED diode on the board and an external speaker module WSR-04489. Snooze activation or alarm dismissal is done through the touch-sensitive TSI panel.

User Manual
  2.1 Current Time Setting
  Setting the current time is done upon program startup or after a restart.
  • Pressing button S1 increments the hours from 0 to 23.
  • S2 accepts the entered hour and proceeds to input minutes.
  • Pressing button S1 increments the minutes from 0 to 59.
  • Pressing S2 again accepts the entered data and returns to the main screen.
  
  2.2 Alarm
  Setting the alarm is possible after inputting the current time (2.1).
  • Pressing button S3 switches the alarm to the setting mode.
  • Entering the alarm time follows the same process as setting the current time (2.1).
  • When the current time matches the alarm time, the alarm enters the waking screen, indicated by a flashing LED and a sound   signal.
  • The alarm can be turned off by touching the left side of the touch panel.
  • Snooze extends the alarm by a specified time (SNOOZE_LENGTH) and is activated by touching the right side of the touch panel. Snooze can be activated a certain number of times (SNOOZE_REPEATS). Once this value is exceeded, the alarm will be permanently disabled.
  
  2.3 Reset
  Reset is possible in any state of the program.
  • Pressing button S4 resets the alarm.

  Keyboard connections:
  ![image](https://github.com/piotrekskt/KL05Z-RTC-alarm/assets/154207023/a7b3c2a9-4c08-4685-8f60-9fce2fd6505d)
