# Wzorce komunikacyjne MPI

Realizacja dwóch zadań skupiających się na zaimplementowaniu określonych schematów komunikacyjnych w programowaniu równoległym.

## Zad 1

Obliczanie NWD wielu liczb jednocześnie z zastosowaniem wzorca _dissemination pattern_.

### Polecenie

Proszę policzyć największy wspólny dzielnik (NWD) poszczególnych liczb całkowitych przechowywanych w różnych procesach używając opisanego poniżej wzorca komunikacyjnego. (W każdym z procesów należy zainicjować lokalną przechowywaną przez niego wartość).

Wzorzec komunikacyjny „upowszechnianie” (dissemination pattern) pozwala każdemu z procesów wyliczyć wartość rozwiązania w maksimum log p krokach komunikacyjnych, gdzie każdy z procesów otrzymuje częściowe wyniki aby wykonać ostateczną ich redukcję na koniec ostatniego przebiegu przesyłu danych.

Jak widać na rys1. (załącznik) zakładamy komunikację pomiędzy procesami po jednokierunkowym okręgu (tzn. prawym sąsiadem n-1 procesu jest proces 0). W pierwszym kroku dane są przesyłane do procesu o 1 większego od procesu wysyłającego, następnie wykonywana jest operacja na danych cząstkowych, w kolejnych krokach do procesu o 2 większego itd. (przesunięcie jest z każdym kolejnym krokiem zwiększane dwukrotnie), aż do pełnej wymiany informacji.

Dla uproszczenia implementacja może ograniczać się do l. procesów będących potęgą 2.

![Pattern visualization](./images/rys_1.png)
