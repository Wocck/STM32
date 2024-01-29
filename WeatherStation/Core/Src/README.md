# PMIK2023Z-Weather Station
Team:
* Wojtek Sekuła
* Piotr Targos

## Wprowadzenie

Jest to projekt na przedmiot "Programowanie Mikrokontrolerów" w semestrze 2023Z. Jego głównym celem jest stworzenie działającej stacji pogodowej z wieloma różnymi funkcjami, które zostaną opisane poniżej.
Projekt działa pod kontrolą płytki NUCLEO-L073RZ.

Głowne cechy płytki NUCLEO-L073RZ:
* Wbudowany programator-debuger ST-LINK/V2-1
* Mikrokontroler STM32L073RZ z: 192 KB pamięci Flash, 20 KB pamięci RAM
* Możliwość rozbudowy za pomocą nakładek Arduino Uno V3
* Złącze Micro-B USB umożliwiające korzystanie z interfejsu ST-LINK, wirtualnego portu COM oraz pamięci masowej
* Obsługa szerokiego zakresu napięć zasilania od 3,3 V do 5 V

## Opis Projektu
Głowne komponenty systemu:
- NUCLEO-L073RZ
- 1.8 inch TFT display
- Temp & Hum Sensor AHT20
- Moduł czytnika kart SD
- ESP WROOM-32 (jako moduł do komunikacji WiFi)

Krótki opis:
Stacja pogodowa, która odczytuje temperaturę oraz wilgotność pomieszczenia, pobiera dane przez moduł WiFi o pogodzie w danej lokalizacji (np. opady, temperatura, ciśnienie, zachmurzenie) następnie wyświetla te dane na wyświetlaczu. Dane będą odświeżane w ustalonych odstępach czasu aby były aktualne. Stację będzie można rozszerzyć o kolejne moduły odczytujące inne informacje. Stacja będzie logowała dane pogodowe na karcie SD.

## Wykaz Realizowanych Funkcji
Kluczowe:
- Odczytywanie/Wyświetlanie danych z czujników
- Pobieranie/Wyświetlanie danych pobranych z internetu przez API
- Zapis danych pogodowych na karcie SD

Opcjonalne:
- Odczyt danych z karty SD (np wyświetlenie historii pogody z ostatnich 7 dni)
- Możliwość wyboru lokalizacji (np. z 5 domyślnych)
- Możliwość dołączeniakolejnych czujników (np. Barometr)
- Wysyłanie danych z czujników przez moduł Bluetooth do telefonu

## Ogólny opis działania algorytmu 
W głownej pętli programu (while) timer będzie generował przerwanie po ustalonym czasie (np. 10s) następnie zczyta dane z czujników oraz pobierze dane o pogodzie z internetu i wyświetli nowe dane na wyświetlaczu a stare zapisze do karty SD.

## Harmonogram Prac
1. (25.10 - 22.11) - Uruchomienie płytki, wgranie programu "hello world", podłączenie czujnika 
2. (22.11 - 6.12)  - Zczytywanie danych z czujnika temperatury i wyświetlanie na ekranie
3. (6.12 - 3.01)   - Zapis danych na kartę SD, połączenie z modułem ESP (WiFi)
4. (3.01 - 20.01)  - Praca nad kodem 
5. (20.01 - 25.01) - Oddanie kompletnego projektu

Podział obowiązków:\
Wojciech    - Software, opieka nad etapami 1, 3, 5\
Piotr       - Hardware, opieka nad etapami 2, 4, 5
