# 2019_LEDvisualiser

Projekt został utworzony w ramach zajęć projektowych
z Podstaw Technik Mikroprocesorowych.  
  
Za cel projektu obrano możliwość komunikacji między płytką
STM32 Discovery, a wyświetlaczem LED 8x32, którego działanie
opiera się o sterownik MAX7219.  
  
Po otrzymaniu przez płytkę ciągu 32 bajtów, których wartości zawierają
się między '0' a '8', przekazuje ona odpowiedni sygnał na wyświetlacz,
celem zapalenia zadanej liczby diod licząc od dolnej krawędzi
wyświetlacza.
