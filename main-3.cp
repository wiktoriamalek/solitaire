//  Projekt nr2 - Techniki Programwania
//
//  Zadanie 5 - Pasjans
//  Maria Rucińska i  Wiktoria  Malek

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <time.h>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
#include <thread>

using namespace std;

string hearts = "\u2661";
string diamond = "\u2662";
string club = "\u2663";
string spades = "\u2660";

class Card {
public:
    unsigned int value;
    string suit; // hearts(r) spades(b) diamond(r) club(b)
    char color; // r-red b-black
    string face; // A-2-10-K
    bool lyingSide = 1; // true - widzę kartę, false - karta jest odwrócona
    Card() {}
    ~Card() {}
};
class Deck {
public:
    Card card;
    vector <Card> deckOfCards;
    vector <string> faces = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    void makeDeck();
    void shuffleDeck();
    Deck() {}
    ~Deck() {}
};
class Solitaire {
public:
    queue <Card> nextCard; // numer 0
    vector <stack <Card>> collectorPile; // numer 8
    vector <vector <Card>> workPile; // numery 1-7
    bool endGame = 0;
    void gameArrangement(Deck);
    void view();
    void movement();
    void move1(Card, int, int);
    void move2(Card, int, int);
    void move3(Card, int, int);
    void move4(Card, int, int);
    bool ifCheck(Card, int, int);
    bool ifCheckThread(bool& result, Card, int, int);
    void menu(Deck);
    Solitaire() {}
    ~Solitaire() {}
};

bool Solitaire::ifCheckThread(bool& result, Card movementCard, int startingPosition, int targetPosition) {
   // bool Solitaire::ifCheck(Card movementCard, int startingPosition, int targetPosition) {
        
        if(startingPosition < 0 || startingPosition > 8) {
            cout << "Zle parametry poczatkowe" << endl;
            return false;
        }
        if(targetPosition < 1 || targetPosition > 8) {
            cout << "Zle parametry docelowe" << endl;
            return false;
        }
        if(targetPosition == startingPosition) {
            cout << "Parametry nie moga byc takie same" << endl;
            return false;
        }
        if(movementCard.color == workPile[targetPosition - 1].back().color) {
            cout << "Nie mozesz polozyc na sobie kart tego samego koloru" << endl;
            return false;
        }
        if(targetPosition > 0 && targetPosition < 8 && movementCard.value != workPile[targetPosition - 1].back().value - 1) {
            cout << "Nie mozesz polozyc karty o takiej wartosci" << endl;
            return false;
        }
        if(startingPosition == 0 && (movementCard.face != nextCard.front().face || movementCard.suit != nextCard.front().suit)) {
            cout << "To nie podana karta" << endl;
            return false;
        }
        if(targetPosition == 8) {
            if(movementCard.suit == hearts && collectorPile[0].top().value != movementCard.value - 1) {
                cout << "Nie mozesz polozyc karty o takiej wartosci" << endl;
                return false;
            }
            if(movementCard.suit == diamond && collectorPile[1].top().value != movementCard.value - 1) {
                cout << "Nie mozesz polozyc karty o takiej wartosci" << endl;
                return false;
            }
            if(movementCard.suit == club && collectorPile[2].top().value != movementCard.value - 1) {
                cout << "Nie mozesz polozyc karty o takiej wartosci" << endl;
                return false;
            }
            if(movementCard.suit == spades && collectorPile[3].top().value != movementCard.value - 1) {
                cout << "Nie mozesz polozyc karty o takiej wartosci" << endl;
                return false;
            }
        
        
        return true;
    }    result = true; // ustawienie wyniku
    return 0;
}

int main() {
    Deck myDeck;
    myDeck.makeDeck();
    myDeck.shuffleDeck();
    
    Solitaire myGame;
    thread t(&Solitaire::ifCheck, &myGame, Card(), 0, 0);
    
    myGame.menu(myDeck);
    t.join();
    return 0;
}



void Deck::makeDeck() {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 13; j++) {
            if(i == 0) {
                card.suit = hearts;
                card.color = 'r';
            } if(i == 1) {
                card.suit = spades;
                card.color = 'b';
            } if(i == 2) {
                card.suit = diamond;
                card.color = 'r';
            } if(i == 3) {
                card.suit = club;
                card.color = 'b';
            }
            card.face = faces[j];
            card.value = j+1;
            deckOfCards.push_back(card);
        }
    }
}
void Deck::shuffleDeck() {
    srand((unsigned) time(NULL));
    for(int i = 0; i < deckOfCards.size(); i++) {
        unsigned int random = rand() % deckOfCards.size();
        swap(deckOfCards[i], deckOfCards[random]);
    }
}

void Solitaire::gameArrangement(Deck myDeck) {
    int counter = 0;
    for(int i = 0; i < 7; i++) {
        vector <Card> storePile;
        for(int j = 0; j < i+1; j++) {
            if(j != i)
                myDeck.deckOfCards[counter].lyingSide = false;
            storePile.push_back(myDeck.deckOfCards[counter]);
            counter++;
        }
        workPile.push_back(storePile);
    }
    for(int i = 28; i < myDeck.deckOfCards.size(); i++) {
        nextCard.push(myDeck.deckOfCards[counter]);
        counter++;
    }
    for(int i = 0; i < 4; i++) {
        stack <Card> winPile;
        collectorPile.push_back(winPile);
    }
}
void Solitaire::view() {
    
    cout << endl << "| 0 |" << setw(2) << nextCard.front().face << " " << nextCard.front().suit << " |" << endl;
    
    for(int i = 0; i < workPile.size(); i++) {
        cout << "| " << i+1 << " |";
        for(int j = 0; j < workPile[i].size(); j++){
            if(workPile[i][j].lyingSide == true)
                cout << setw(2) << workPile[i][j].face << " " << workPile[i][j].suit << " |";
            else
                cout << "=====|";
        }
        cout << endl;
    }
    cout << "| 8 |";
    
    if(collectorPile[0].empty())
        cout << "|---" << hearts << "---|  ";
    else
        cout << "| " << setw(2) << collectorPile[0].top().face << " " << collectorPile[0].top().suit << "  |";
    if(collectorPile[1].empty())
        cout << "|---" << diamond << "---|  ";
    else
        cout << "| " << setw(2) << collectorPile[1].top().face << " " << collectorPile[1].top().suit << "  |";
    if(collectorPile[2].empty())
        cout << "|---" << club << "---|  ";
    else
        cout << "| " << setw(2) << collectorPile[2].top().face << " " << collectorPile[2].top().suit << "  |";
    if(collectorPile[3].empty())
        cout << "|---" << spades << "---|  ";
    else
        cout << "| " << setw(2) << collectorPile[3].top().face << " " << collectorPile[3].top().suit << "  |";
}
void Solitaire::movement() {
    
    while(endGame == false) {
        view();
        cout << endl << "Twój ruch:" << endl;
        
        Card movementCard;
        int startingPosition;
        int targetPosition;
        char suitCard;
        cin >> startingPosition;
        cin >> movementCard.face;
        cin >> suitCard;
        cin >> targetPosition;
        
        if(suitCard == 'h') {
            movementCard.suit = hearts;
            movementCard.color = 'r';
        }
        else if(suitCard == 'd') {
            movementCard.suit = diamond;
            movementCard.color = 'r';
        }
        else if(suitCard == 'c') {
            movementCard.suit = club;
            movementCard.color = 'b';
        }
        else if(suitCard == 's') {
            movementCard.suit = spades;
            movementCard.color = 'b';
        }
        
        if(movementCard.face == "A")
            movementCard.value = 1;
        else if(movementCard.face == "2")
            movementCard.value = 2;
        else if(movementCard.face == "3")
            movementCard.value = 3;
        else if(movementCard.face == "4")
            movementCard.value = 4;
        else if(movementCard.face == "5")
            movementCard.value = 5;
        else if(movementCard.face == "6")
            movementCard.value = 6;
        else if(movementCard.face == "7")
            movementCard.value = 7;
        else if(movementCard.face == "8")
            movementCard.value = 8;
        else if(movementCard.face == "9")
            movementCard.value = 9;
        else if(movementCard.face == "10")
            movementCard.value = 10;
        else if(movementCard.face == "J")
            movementCard.value = 11;
        else if(movementCard.face == "Q")
            movementCard.value = 12;
        else if(movementCard.face == "K")
            movementCard.value = 13;
        
        if(ifCheck(movementCard, startingPosition, targetPosition)) {
            
            if(startingPosition > 0 && startingPosition < 8 && targetPosition > 0 && targetPosition < 8)
                move1(movementCard, startingPosition, targetPosition);
            if(startingPosition == 0 && targetPosition < 8)
                move2(movementCard, startingPosition, targetPosition);
            if(startingPosition > 0 && startingPosition < 8 && targetPosition == 8)
                move3(movementCard, startingPosition, targetPosition);
            if(startingPosition == 0 && targetPosition == 8)
                move4(movementCard, startingPosition, targetPosition);
        }
        else {
            cout << "Bledny ruch" << endl;
            cout << "Jesli chcesz wyjsc nacisnij 1, jesli powtarzasz  ruch nacisnij 0: ";
            char exit;
            cin >> exit;
            if(exit == '1')
                endGame = true;
        }
        
        if(collectorPile[0].size() == 13 && collectorPile[1].size() == 13 && collectorPile[2].size() == 13 && collectorPile[3].size() == 13) {
            cout << "GRATULACJE! WYGRALES GRE!" << endl;
            endGame = true;
        }
    }
}
bool Solitaire::ifCheck(Card movementCard, int startingPosition, int targetPosition) {
    
    if(startingPosition < 0 || startingPosition > 8) {
        cout << "Zle parametry poczatkowe" << endl;
        return false;
    }
    if(targetPosition < 1 || targetPosition > 8) {
        cout << "Zle parametry docelowe" << endl;
        return false;
    }
    if(targetPosition == startingPosition) {
        cout << "Parametry nie moga byc takie same" << endl;
        return false;
    }
    if(movementCard.color == workPile[targetPosition - 1].back().color) {
        cout << "Nie mozesz polozyc na sobie kart tego samego koloru" << endl;
        return false;
    }
    if(targetPosition > 0 && targetPosition < 8 && movementCard.value != workPile[targetPosition - 1].back().value - 1) {
        cout << "Nie mozesz polozyc karty o takiej wartosci" << endl;
        return false;
    }
    if(startingPosition == 0 && (movementCard.face != nextCard.front().face || movementCard.suit != nextCard.front().suit)) {
        cout << "To nie podana karta" << endl;
        return false;
    }
    if(targetPosition == 8) {
        if(movementCard.suit == hearts && collectorPile[0].top().value != movementCard.value - 1) {
            cout << "Nie mozesz polozyc karty o takiej wartosci" << endl;
            return false;
        }
        if(movementCard.suit == diamond && collectorPile[1].top().value != movementCard.value - 1) {
            cout << "Nie mozesz polozyc karty o takiej wartosci" << endl;
            return false;
        }
        if(movementCard.suit == club && collectorPile[2].top().value != movementCard.value - 1) {
            cout << "Nie mozesz polozyc karty o takiej wartosci" << endl;
            return false;
        }
        if(movementCard.suit == spades && collectorPile[3].top().value != movementCard.value - 1) {
            cout << "Nie mozesz polozyc karty o takiej wartosci" << endl;
            return false;
        }
    }
    
    return true;
}
void Solitaire::move1(Card movementCard, int startingPosition, int targetPosition) {
    
    int tempPosition = 0;
    Card temp = workPile[startingPosition - 1][0];
    for(int i = 0; i < workPile[startingPosition - 1].size(); i++) {
        temp = workPile[startingPosition - 1][i];
        if(temp.face == movementCard.face && temp.suit == movementCard.suit)
            tempPosition = i;
    }
    int counter = 0;
    for(int i = tempPosition; i < workPile[startingPosition - 1].size(); i++) {
        workPile[targetPosition - 1].push_back(workPile[startingPosition - 1][i]);
        counter++;
    }
    for(int i = 0; i < counter; i++) {
        workPile[startingPosition - 1].pop_back();
        if(workPile[startingPosition - 1].size() > 0)
            workPile[startingPosition - 1].back().lyingSide = true;
    }
}
void Solitaire::move2(Card movementCard, int startingPosition, int targetPosition) {
    
    workPile[targetPosition - 1].push_back(movementCard);
    if(!nextCard.empty())
        nextCard.pop();
}
void Solitaire::move3(Card movementCard, int startingPosition, int targetPosition) {
    
    if(movementCard.suit == hearts)
        collectorPile[0].push(movementCard);
    if(movementCard.suit == diamond)
        collectorPile[1].push(movementCard);
    if(movementCard.suit == club)
        collectorPile[2].push(movementCard);
    if(movementCard.suit == spades)
        collectorPile[3].push(movementCard);
    
    workPile[startingPosition - 1].pop_back();
}
void Solitaire::move4(Card movementCard, int startingPosition, int targetPosition) {
    
    if(movementCard.suit == hearts)
        collectorPile[0].push(movementCard);
    if(movementCard.suit == diamond)
        collectorPile[1].push(movementCard);
    if(movementCard.suit == club)
        collectorPile[2].push(movementCard);
    if(movementCard.suit == spades)
        collectorPile[3].push(movementCard);
    
    nextCard.pop();
}
void Solitaire::menu(Deck myDeck) {
    int choice;
    while(true){
        cout << "-------------------------------------" << endl;
        cout << "------------MENU GLOWNE--------------" << endl;
        cout << "-------------PASJANS-----------------" << endl;
        cout << "Maria Rucińska i Wiktoria Malek" << endl;
        cout << "1. Rozpocznij nowa gre" << endl;
        cout << "2. Zasady gry" << endl;
        cout << "3. Wyjdz z gry" << endl;
        cout << "Wybierz opcje: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Rozpoczynamy nową grę..." << endl;
                
                gameArrangement(myDeck);
                movement();
                
                break;
            case 2:
                cout << "Zasady gry:" << endl;
                cout << "Celem gry jest ułożenie wszystkich kart na stosach w odpowiedniej kolejności, zaczynając od asa i kończąc na królu.Gra składa się z 52 kart, podzielonych na 7 stosów, z których pierwszy ma jedną kartę, drugi - dwie karty, trzeci - trzy karty i tak dalej, aż do siódmego stosu, który ma siedem kart. Pozostałe karty są układane na stosie talii. Gracz może przekładać karty między stosami, ale tylko wtedy, gdy karta, którą chce przenieść, jest o jeden punkt niższa i ma inny kolor niż karta, na którą chce ją położyć. Jeśli gracz nie ma żadnych ruchów, może dobrać jedną kartę ze stosu talii. Może to zrobić tylko raz. Grę można wygrać, jeśli uda się ułożyć wszystkie karty na stosach w odpowiedniej kolejności. Jeśli gracz nie może wykonać żadnego ruchu i nie ma już kart w talii, gra kończy się przegraną. Istnieją różne wersje gry pasjans, różniące się liczbą stosów i zasadami, ale podstawowe zasady pozostają takie same.To są podstawowe zasady gry w pasjansa. Gra ta wymaga cierpliwości, umiejętności logicznego myślenia i strategii." << endl;
                break;
            case 3:
                cout << "Do widzenia!" << endl;
                exit(0);
                break;
            default:
                cout << "Nieprawidłowa opcja. Spróbuj ponownie." << endl;
                menu(myDeck);
                break;
        }
    }
}
