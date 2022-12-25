#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <iomanip> 

using namespace std;

// Card struct
struct Card {
	string suit, description;
	int rank, value;

	Card() {
		suit = description = "";
		rank = value = 0;
	}
};

// Card Array struct
struct CardArray {
	Card* arr;
	int arrSize, arrUsed;

	CardArray() {
		arr = nullptr;
		arrSize = arrUsed = 0;
	}
};

// Function Prototypes
void getNewDeck(CardArray& deck);
void printDeck(const CardArray& deck);
void shuffleDeck(CardArray& deck);
int blackjack(CardArray& deck);
void deal(CardArray& deck, CardArray& hand);
int handScore(CardArray deck);
void showHand(CardArray hand, string s);
void playerInput(char & playerChoice);
void playGames(CardArray& deck);
void advice(CardArray playerHand, CardArray dealerHand);

int main() {

	srand(time(0));

	// Create new deck
	CardArray deck;
	getNewDeck(deck);

	// Print deck
	cout << "  Deck:" << endl;
	printDeck(deck);

	// Shuffle deck
	shuffleDeck(deck);

	// Print shuffled deck
	cout << endl << endl << "  Shuffled Deck:" << endl;
	printDeck(deck);

	// Play blackjack game
	playGames(deck);

	delete[] deck.arr;
}

// Populates a CardArray with a standard deck of cards
void getNewDeck(CardArray& deck) {

	const int deckSize = 52;
	deck.arrSize = deck.arrUsed = deckSize;

	// create an array of 52 cards in dynamic memory
	deck.arr = new Card[deck.arrSize];

	string suits[4] = { "S","H","D","C" };
	int ranks[13] = { 1,2,3,4,5,6,7,8,9,10,11,12,13 };
	int values[13] = { 11,2,3,4,5,6,7,8,9,10,10,10,10 };
	string descriptions[13] = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };

	int cardCount = 0;

	for (int i = 0; i < 4; i++) {

		for (int n = 0; n < 13; n++) {
			deck.arr[cardCount].suit = suits[i];
			deck.arr[cardCount].rank = ranks[n];
			deck.arr[cardCount].value = values[n];
			deck.arr[cardCount].description = descriptions[n];
			cardCount++;
		}

	}
}

// Prints a deck of cards
void printDeck(const CardArray& deck) {
	int count = 1;

	for (int i = 0; i < deck.arrUsed; i++) {
		cout << setw(3) << deck.arr[i].description << deck.arr[i].suit;

		if (count % 13 == 0) {
			cout << endl;
		}

		count++;
}

}

// Shuffles a deck of cards
void shuffleDeck(CardArray& deck) {

	for (int i = 0; i < 52; i++) {
		int random = rand() % 52;
		Card temp = deck.arr[random];
		deck.arr[random] = deck.arr[i];
		deck.arr[i] = temp;
	}

}

// Asks player if they want to play a hand of blackjack
void playGames(CardArray& deck) {
	int result = 0;
	int games = 0;
	int wins = 0;
	int losses = 0;
	int draws = 0;
	char playerChoice = 'y';

	cout << endl << "BLACK JACK" << endl;
	cout << "==========" << endl;
	cout << "Do you want to play a hand of blackjack (y/n)? ";
	cin >> playerChoice;

	// Input checking
	while (playerChoice != 'y' && playerChoice != 'Y' && playerChoice != 'n' && playerChoice != 'N') {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Please enter a valid choice: ";
		cin >> playerChoice;
	}

	// If player does not want to play
	if (playerChoice == 'n' || playerChoice == 'N') {
		cout << endl << "Goodbye!" << endl;
		return;
	}

	// Plays blackjack until user doesn't want to play another hand
	while (playerChoice == 'y' || playerChoice == 'Y') {

		result = blackjack(deck);

		games++;

		// Count record
		if (result == 1) {
			wins++;
		}
		else if (result == -1) {
			losses++;
		}
		else draws++;

		cout << endl << "----------------------------------------------------------------" << endl;
		cout << "Do you want to play another hand of blackjack (y/n)? ";
		cin >> playerChoice;

		if (playerChoice != 'y' && playerChoice != 'Y' && playerChoice != 'n' && playerChoice != 'N') {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Please enter a valid choice: ";
			cin >> playerChoice;
		}
	}

	cout << endl << "Thanks for playing, you played " << games;
	if (games == 1) {
		cout << " game and your record was:" << endl;
	}
	else {
		cout << " games and your record was:" << endl;
	}
	cout << setw(7) << "wins:" << setw(3) << wins << endl;
	cout << setw(7) << "losses:" << setw(3) << losses << endl;
	cout << setw(7) << "draws:" << setw(3) << draws << endl;
}

// Plays a game of blackjack
int blackjack(CardArray& deck) {

	// Create player and dealer hands
	CardArray playerHand;
	CardArray dealerHand;

	const int handSize = 12;
	playerHand.arrSize = dealerHand.arrSize = handSize;
	
	playerHand.arr = new Card[handSize];
	dealerHand.arr = new Card[handSize];

	string pString = "-Player-";
	string dString = "*Dealer*";

	// Deal first card
	cout << endl << "Deal first card" << endl;
	cout << "---------------" << endl;

	// Print player hand
	deal(deck, playerHand);
	showHand(playerHand, pString);

	// Print dealer hand
	deal(deck, dealerHand);
	showHand(dealerHand, dString);
	cout << endl; 

	// Deal second card
	cout << "Deal second card" << endl;
	cout << "----------------" << endl;

	// Print player hand
	deal(deck, playerHand);
	showHand(playerHand, pString);

	// Print dealer hand with 2nd card hidden
	cout << "*Dealer* :";
	deal(deck, dealerHand);
	cout << setw(3) << dealerHand.arr[0].description << dealerHand.arr[0].suit;
	cout << setw(4) << "??" << endl;

	// Winning/losing with the first 2 cards
	if (handScore(playerHand) == 21) {
		showHand(dealerHand, dString);

		// Draw case
		if (handScore(dealerHand) == 21) {
			cout << endl << "Game is tied." << endl;
			return 0;
		}
		// Win case
		else {
			cout << endl << "Blackjack! You win." << endl;
			return 1;
		}
	}

	// Player is bust
	else if (handScore(playerHand) > 21) {
		showHand(dealerHand, dString);
		cout << endl << "Bust! You lose." << endl;
	}

	// Dealer wins
	if (handScore(dealerHand) == 21) {
		showHand(dealerHand, dString);
		cout << endl << "Dealer wins, you lose." << endl;
		return -1; 
	}

	// Dealing to player
	cout << endl << "Dealing to player" << endl;
	cout << "-----------------" << endl;
	char playerChoice;
	advice(playerHand, dealerHand);
	playerInput(playerChoice);

	// Player hits
	while (playerChoice == 'h' || playerChoice == 'H') {
		deal(deck, playerHand);

		// Print player hand
		showHand(playerHand, pString);

		// Player goes bust
		if (handScore(playerHand) > 21) {
			showHand(dealerHand, dString);
			cout << endl << "Bust! You lose." << endl;
			return -1;
		}
		advice(playerHand, dealerHand);
		playerInput(playerChoice);
	}

	// Player stands
	if (playerChoice == 's') {

		// Dealing to dealer
		cout << endl << "Dealing to dealer" << endl;
		cout << "-----------------" << endl;
		showHand(dealerHand, dString);

		// Dealer is dealt a card
		while (handScore(dealerHand) < 17) {
			deal(deck, dealerHand);
			showHand(dealerHand, dString);
		}

		// Dealer is bust
		if (handScore(dealerHand) > 21) {
			cout << endl << "Dealer is bust, you win." << endl;
			return 1;
		}

		// Highest score wins
		if (handScore(dealerHand) >= 17 && handScore(dealerHand) <= 21) {
			cout << endl << "player score = " << handScore(playerHand);
			cout << ", dealer score = " << handScore(dealerHand) << endl;

			// Draw case
			if (handScore(dealerHand) == handScore(playerHand)) {
				cout << endl << "Game is tied." << endl;
				return 0;
			}
			// Player wins
			else if (handScore(playerHand) > handScore(dealerHand)) {
				cout << endl << "You have a higher score, you win." << endl;
				return 1; 
			}
			else {
				cout << endl << "Dealer has a higher score, you lose." << endl;
				return -1;
			}
		}

	}
	

	delete[] playerHand.arr;
	delete[] dealerHand.arr;
	return 0;
}

// Deals a card for the player or dealer's hand
void deal(CardArray& deck, CardArray& hand) {
	// Give one card to player/dealer hand
	hand.arr[hand.arrUsed] = deck.arr[deck.arrUsed - 1];

	// Adjust the current number of cards in both decks
	hand.arrUsed += 1;
	deck.arrUsed -= 1;

	if (deck.arrUsed == 0) {
		delete[] deck.arr;
		getNewDeck(deck);
		shuffleDeck(deck);
	}
}

// Counts the score of a hand
int handScore(CardArray deck) {
	int count = 0;
	int aceValue = 11;

	// Ensure ace is counted as 11
	for (int i = 0; i < deck.arrUsed; i++) {

		if (deck.arr[i].description == "A") {
			deck.arr[i].value = 11;
		}
	}

	// Count score if ace is 11
	for (int i = 0; i < deck.arrUsed; i++) {
		count += deck.arr[i].value;
	}

	// Chance ace value to 1 if 11 causes a bust
	if (count > 21) {
		aceValue = 1;
		count = 0;
	}
	else {
		return count;
	}
																																	
	// Change ace value to 1
	for (int i = 0; i < deck.arrUsed; i++) {

		if (deck.arr[i].description == "A") {
			deck.arr[i].value = 1;
		}
	}

	// Count score if ace is 1
	for (int i = 0; i < deck.arrUsed; i++) {
		count += deck.arr[i].value;
	}

	return count;
}

// Prints the cards in a player/dealer's hand
void showHand(CardArray hand, string s) {
	cout << s << " :";
	printDeck(hand);
	cout << endl;
}

// Get player input if they want to hit or stand
void playerInput(char & playerChoice) {
	cout << "Enter h to hit or s to stand: ";
	cin >> playerChoice;

	// Input checking
	while (playerChoice != 'h' && playerChoice != 'H' && playerChoice != 's' && playerChoice != 'S') {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Please enter a valid choice: ";
		cin >> playerChoice;
	}
}

// Give advice to player whether to hit or stand
void advice(CardArray playerHand, CardArray dealerHand) {
	string dealerCard = dealerHand.arr[0].description;
	int playerScore = handScore(playerHand);
	cout << endl;
	// Check if player has a soft hand (there is an ace)
	for (int i = 0; i < 2; i++) {

		if (playerHand.arr[i].description == "A") {

			// Give advice based on player score
			if (playerScore < 18) {
				cout << "You should hit because your score is still " << playerScore << ", and you have a soft hand (an ace in your hand)." << endl;
				cout << "The value of the ace is in your favour, so if you hit you are unlikely to go bust." << endl;
				return;
			}
			else {
				cout << "You should stand because you score is " << playerScore << ", if you hit you are likely to go bust." << endl;
				return;
			}
		}
	}
	
	// Assess the dealer's showing card (good or poor)
	// Good cards
	if (dealerCard == "A" || dealerCard == "J" || dealerCard == "Q" || dealerCard == "K" || dealerCard == "7" || dealerCard == "8" || dealerCard == "9") {

		// Give advice based on player score
		if (playerScore < 17) {
			cout << "You should hit because your score is still " << playerScore << ", and the dealer has a good card." << endl;
			cout << "Hitting will maximize your chances of winning against the dealer." << endl;
			return;
		}
		else {
			cout << "You should stand because you score is " << playerScore << ", if you hit you are likely to go bust." << endl;
			return;
		}
	}

	// Poor cards
	else {
		
		// Give advice based on player score
		if (playerScore < 12) {
			cout << "You should hit because your score is still " << playerScore << ", if you hit you are unlikely to go bust." << endl;
			return;
		}
		else {
			cout << "You should stand because you score is " << playerScore << ". You should not take any risk of going bust";
			cout << " since the dealer has poor cards, meaning they are likely to go bust" << endl;
			return;
		}
	}
}
