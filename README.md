# alcapower_remote
Alcapower remote con ESP8266

Il comando remoto è come quello in foto:

![Screenshot](inverter_1_0.jpg)

Ci sono 2 modelli: 12V e 24V. Io ho il 24V collegato all'inverter.
In parallelo ai fili del comando remoto originale ho collegato un ESP8266 in questo modo:

![Screenshot](schematic.jpg)

Se non si vuole usare il comando remoto originale è meglio mettere dei pulldown di 100K sui pin 4 e 5 del connettore.

Caricando lo sketch nell'ESP8266 si apre una pagina (http://xx.xx.xx.xx/ con xx.xx.xx.xx che dipende dal router wifi) come questa:

![Screenshot](webapp.jpg)

Premendo il pulsante rotondo si simula la pressione del tasto del comando remoto.
Bisogna ricordarsi di inserire i giusti dati del proprio router wifi.
