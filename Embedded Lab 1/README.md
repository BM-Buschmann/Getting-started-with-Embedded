# Embedded Lab 1 - Labor Microcomputertechnik 1

## German

Aufgabenstellung:

Die Grundausstattung von Mikrocontrollern sind die digitalen I/O-Ports. Je nach Familie besitzt ein Mikrocontroller mehrere universell einsetzbare I/O-Ports. Ein Port ist hierbei die Gruppierung von meist acht bidirektionalen I/O-Anschlüssen, welche über ein Register des Prozessors angesteuert wird. Die jeweiligen Anschlüsse werden als Pins bezeichnet. Da ein Mikrocontroller in der Regel deutlich mehr Peripherie-Funktionalität als Pins besitzt, müssen diese über eine funktionelle Mehrfachbelegung gesteuert werden. Dadurch teilen sich verschiedene interne Peripheriemodule einzelne Pins. Durch die Konfiguration spezieller Register wird dabei festgelegt, welche Funktion ein Pin zu einem gegebenen Zeitpunkt besitzt.

In **Versuch 1** soll das Konfigurieren der Register zum Steuern der digitalen I/O-Ports näher betrachtet werden. Zuerst muss ein Digitalport initialisiert werden. Dazu muss in dem **FunctionSelect-Register (PxSEL)** die gewünschte Funktionalität der Portpins definiert werden. Als nächstes muss dem Mikrocontroller mitgeteilt werden, auf welche Portpins lesend und auf welche schreibend zugegriffen werden soll. Dazu müssen die entsprechenden Bits in dem **Richtungsregister PxDIR** gesetzt werden. Ist die Richtung (als Eingang oder Ausgang) gesetzt, kann der Status (High/Low) des jeweiligen Portpins durch das **PxOUT Register** geändert werden.

**Aufgabe 1:**
a) Verbinden Sie **JP8:LED_RT** mit **CON3:P1.7** und lassen Sie die rote LED sichtbar mit etwa **1,25 Hz** blinken. Der Duty Cycle sollte etwa **50%** betragen. (3 Pkt.)
b) Verbinden Sie **JP8:LED_GN** mit **CON3:P1.0** und lassen Sie sie mit der vierfachen Frequenz der roten LED blinken. Der Duty Cycle soll etwa **75%** betragen. (2 Pkt.)
c) Fügen Sie Ihrem Programm einen oder zwei Screenshots bei, die die beiden Frequenzen aus a und b im **Logic Analyzer** zeigen. Benennen Sie den ersten der Kanäle mit Ihrem Nachnamen. Den zweiten mit Ihrem Vornamen. Toleranz der Frequenzen +/-10%. (2 Pkt.)
d) Geben Sie, wann immer Daten über die serielle Schnittstelle empfangen werden (und auch nur dann), den aktuellen Zustand der roten LED über selbige Schnittstelle aus. Verwenden Sie hierfür die vordefinierten Funktionen aus dem **Template**, welches Sie auch im Anhang Ihres Begleitdokuments finden. (2 Pkt.)

## English

Task Description:

The basic equipment of microcontrollers is the digital I/O ports. Depending on the family, a microcontroller has several universally applicable I/O ports. A port is a grouping of mostly eight bidirectional I/O ports controlled by a processor register. These individual ports are referred to as pins. Since a microcontroller typically has significantly more peripheral functionality than pins, these must be controlled through functional multiplexing. This means that various internal peripheral modules share individual pins. The configuration of special registers determines the function of a pin at any given time.

In **Experiment 1** focuses on configuring the registers to control digital I/O ports. First, a digital port must be initialized. To do this, the desired functionality of the port pins must be defined in the **FunctionSelect register (PxSEL)**. Next, the microcontroller needs to be informed about which port pins to access for reading and which for writing. This involves setting the corresponding bits in the **Direction register PxDIR**. Once the direction (as input or output) is set, the status (High/Low) of the respective port pin can be changed using the **PxOUT register**.

**Task 1:**
a) Connect **JP8:LED_RT** to **CON3:P1.7** and make the red LED visible by blinking at approximately **1.25 Hz**. The duty cycle should be around **50%**. (3 Pts.)
b) Connect **JP8:LED_GN** to **CON3:P1.0** and make it blink at four times the frequency of the red LED. The duty cycle should be around **75%**. (2 Pts.)
c) Include one or two screenshots in your program that show the two frequencies from a and b in the **Logic Analyzer**. Name the first channel with your last name. The second with your first name. Frequency tolerance +/-10%. (2 Pts.)
d) Whenever data is received via the serial interface (and only then), output the current state of the red LED through the same interface. Use the predefined functions from the **Template**, which you can find in the appendix of your accompanying document. (2 Pts.)