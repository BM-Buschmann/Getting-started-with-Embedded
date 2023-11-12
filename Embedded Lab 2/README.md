# Embedded Lab 2 - Labor Microcomputertechnik 2

## German

Aufgabenstellung:

In **Versuch 1** wurden die digitalen I/O-Portspins nur als Ausgangspins verwendet. In diesem Versuch sollen diese auch als Eingangspins verwendet werden. Auf der Praktikumsplatine finden Sie hierzu am unteren Rand die drei Taster PB5, PB6 und PB7. Diese können über die Pins an X1 mit dem Mikrocontroller verbunden werden. Beim Drücken dieser Taster wird eine physikalische Verbindung mit GND hergestellt. Nach entsprechender Konfiguration von **PxDIR** (und evtl. **PxSEL**) können Sie das an einem Portpin anliegende Signal über das Eingangsregister **PxIN1** abfragen. Beachten Sie auch, wie die Taster beschaltet sind. Durch das **PxREN-Register2** können Pullup-/Pulldown Widerstände intern an den I/O-Ports geschaltet werden.

**Hinweise:**
Sofern nicht anders vermerkt, sollen Sie in diesem Versuch alle Aufgaben mit Polling lösen, d.h. schreiben Sie vorerst keine Interruptroutine, sondern fragen Sie den entsprechenden Pin aktiv in Ihrem Programm ab.
Sofern nicht anders vermerkt, sollen Sie alle Teilversuche eines Übungsblattes immer in einem Projekt bearbeiten. Die geforderten Teilaufgaben sollen somit alle gleichzeitig ablaufen.

**Aufgabe 1:**
Verbinden Sie den Taster PB5 mit **CON3:P1.5**, PB6 mit **CON3:P1.6** sowie Taster PB7 mit CON3:P1.7. Verbinden Sie des Weiteren die rote LED (JP8- LED rt) mit **CON3:P3.0**, die grüne LED (JP8-LED gn) mit **CON3:P3.1** und den Lautsprecher (X1-BUZZER) mit **CON3:P3.2**.
a) Schreiben Sie ein Programm, in dem der Taster PB5 abgefragt wird. Solange der Taster gedrückt wird, soll die rote LED leuchten. Wird die Taste losgelassen, soll sie erlöschen. (2 Pkt.)
b) Ergänzen Sie Ihr Programm so, dass der Taster PB6 abgefragt wird. Sobald der Taster PB6 gedrückt wurde, soll die grüne LED einmalig kurz (250ms) aufblinken. (“einmalig” bezieht sich hierbei darauf, dass die LED nicht mehrfach blinken soll, solange der Taster gehalten wird. Sollte der Taster jedoch losgelassen und zu einem späteren Zeitpunkt wieder gedrückt werden, soll die LED erneut aufleuchten). (2 Pkt.)
c) Ergänzen Sie Ihr Programm weiter: Schreiben Sie eine Funktion „Gong“. In dieser sollen nach einander die Töne g‘ (396Hz), e´(330Hz) und c´(264Hz) erzeugt werden. Der zeitliche Ablauf ist vorgegeben. Die Tonlänge soll bei jeweils 900ms liegen, die Pause zwischen den Tönen und auch nach der Tonfolge soll 100ms sein. Solange die Tasten PB5 und PB6 gleichzeitig gedrückt werden, wird die Funktion aufgerufen und am Buzzer die Tonfolge ausgegeben. Erklingen Töne, soll parallel die rote LED leuchten. In den Pausen nicht. (Hinweis: Dadurch muss an der roten LED eine Frequenz von 1Hz mit Dutycycle 90% erzeugt werden.) Wird nur PB5 gedrückt, soll sich die rote LED weiterhin wie in a) verhalten. Wird nur PB6 gedrückt, soll sich die rote LED weiterhin wie in b) verhalten. Verwenden Sie keine Timer oder Interrupts dafür. (3 Pkt.) 
d) Nutzen Sie für PB7 einen Interrupt (s. Listing 1), aber fragen Sie PB5 und PB6 weiterhin mittels Polling ab. Bei jedem Drücken von PB7 soll die blaue LED (an Anschluss: X1: Heater) über Pin **CON3:P3.3** getoggelt werden. (2 Pkt.)

## English

Task Description:

In **Experiment 1**, the digital I/O port pins were only used as output pins. In this experiment, they should also be used as input pins. On the practice board, you will find the three buttons PB5, PB6, and PB7 at the bottom. These can be connected to the microcontroller via the pins at X1. When these buttons are pressed, a physical connection with GND is established. After configuring **PxDIR** (and possibly **PxSEL**), you can query the signal applied to a port pin using the input register **PxIN1**. Also, pay attention to how the buttons are connected. Pull-up/pull-down resistors can be internally connected to the I/O ports through the **PxREN register**.

**Notes:**
Unless otherwise stated, in this experiment, you should solve all tasks using polling, i.e., do not write an interrupt routine initially, but actively query the corresponding pin in your program.
Unless otherwise stated, you should always work on all sub-tasks of an exercise sheet in one project. The required subtasks should thus all run simultaneously.

**Task 1:**
Connect button PB5 to **CON3:P1.5**, PB6 to **CON3:P1.6**, and button PB7 to **CON3:P1.7**. Also, connect the red LED (JP8- LED rt) to **CON3:P3.0**, the green LED (JP8-LED gn) to

 **CON3:P3.1**, and the speaker (X1-BUZZER) to **CON3:P3.2**.
a) Write a program that queries button PB5. As long as the button is pressed, the red LED should light up. When the button is released, it should go out. (2 Pts.)
b) Add to your program so that button PB6 is queried. As soon as button PB6 is pressed, the green LED should flash briefly (250ms). ("Once" means that the LED should not blink multiple times as long as the button is held. However, if the button is released and pressed again at a later time, the LED should light up again). (2 Pts.)
c) Further supplement your program: Write a function "Gong". In this function, the tones g‘ (396Hz), e‘ (330Hz), and c‘ (264Hz) should be generated one after the other. The timing is predetermined. The tone length should be 900ms each, the pause between the tones, and also after the tone sequence should be 100ms. As long as buttons PB5 and PB6 are pressed simultaneously, the function will be called, and the tone sequence will be output on the buzzer. When tones sound, the red LED should light up in parallel. Not in the breaks. (Note: This must generate a frequency of 1Hz with a duty cycle of 90% on the red LED.) If only PB5 is pressed, the red LED should continue to behave as in a). If only PB6 is pressed, the red LED should continue to behave as in b). Do not use timers or interrupts for this. (3 Pts.)
d) Use an interrupt for PB7 (see Listing 1), but continue to query PB5 and PB6 using polling. Every time PB7 is pressed, the blue LED (at connection: X1: Heater) should be toggled via pin **CON3:P3.3**. (2 Pts.)