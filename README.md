# Techniki Internetowe - Projekt - System zbierania statystyk częstości zapytań do serwerów FTP (FTP Stattr)
Projekt wykonany w ramach przedmiotu TIN (Techniki Internetowe) w semestrze 2015L (6 semestr) na Wydziale Elektroniki i Technik Informacyjnych (EiTI) Politechniki Warszawskiej.

### Opis projektu
Ideą projektu jest stworzenie systemu obserwującego predefiniowany zbiór maszyn. System pozwala na inicjację i zatrzymywanie pomiarów oraz zbieranie danych w dowolnym momencie. Na każdej obserwowanej maszynie instalowany
jest agent, z którym komunikuje się serwer zarządzający.

_(Reszta dostępna w [dokumentacji końcowej](https://github.com/mdziekon/eiti-tin-ftp-stattr/raw/master/docs/final-documentation.pdf))_

Kompilacja i uruchomienie
---

### Wymagania
* Kompilator wspierający standard **C++11** (``clang`` / ``gcc``)
* Biblioteka **Boost** (``libboost-dev``)
* Narzędzie **Scons** (``scons``)

### Instrukcja
#### Przygotowanie repozytorium
* Podpięcie klucza pod dowolne konto GitHub (aby umożliwić poprawne pobranie zależności)
* Pobranie zależności do folderu repozytorium:
  ``git submodule init && git submodule update``

#### Kompilacja
* Budowa wszystkich aplikacji kompilowanych    
  ``scons`` lub ``scons all``
* Budowa poszczególnych aplikacji    
  _(Dostępne aplikacje: **agent**, **supervisor**, **terminal**)_  
  ``scons <aplikacja>``

#### Instalacja aplikacji klienta
* ``sudo apt-get install nodejs nodejs-legacy npm``
* ``npm install bower grunt grunt-cli -g``
* ``npm install`` (wywołane z poziomu katalogu "webclient")
* ``bower install`` (wywołane z poziomu katalogu "webclient")
* ``grunt serve`` (wywołane z poziomu katalogu "webclient", musi być ciągle włączone, ponieważ
stanowi również rolę serwera plików)

#### Uruchomienie
* Zarządca: ``./build/debug/supervisor/supervisor``
* Agent: ``sudo ./build/debug/agent/agent``
* Terminal: ``./build/debug/terminal/terminal``
* WebClient: Uruchomienie strony ``localhost:9000`` w przeglądarce  
  (należy pamiętać o wcześniejszym uruchomieniu polecenia ``grunt serve``, patrz instrukcja wyżej)

### Testowane na:
* ``Ubuntu 14.10`` + ``Clang 3.5.0-4ubuntu2``
