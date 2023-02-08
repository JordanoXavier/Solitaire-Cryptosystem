# Solitaire is a cryptosystem designed by Bruce Schneier for writer Neal Stephenson to use in his book Cryptonomicon. This system is manual, the code is a C implementation of it.


<div align="center">
<img src="https://user-images.githubusercontent.com/112017191/217660340-b350ca52-495a-4163-a4c7-fbea0b0b0ade.png" width="700px" />
</div> 


## Encryption:
1. remove non-alphabetic characters (spaces, punctuation, numbers), leaving only the letters (a-z) of the message (plaintext)
2. convert each letter to its numeric value (a=1, b=2, ..., z=26)
3. generate a keystream value for each letter, using the algorithm below
4. add each numeric letter value in the message to the corresponding keystream value, subtracting 26 if the result is greater than 26 (modular arithmetic)
5. convert the resulting numbers into letters. This final string is the ciphertext.
Example:
initial setup: sorted deck with jokers at the end
ace of clubs, 2 of clubs, ..., king of clubs, ace of diamonds,..., king of diamonds, ace of hearts,..., king of hearts, ace of spades,..., king of spades, joker A, joker B
message: AAAAAAAAAAA
ciphertext: EXKYIZSGEH


## Decryption:
 1. convert each letter to its numerical value (a=1, b=2, ..., z=26)
 2. generate a keystream value for each letter, using the algorithm below
 3. Subtract each numeric letter value from the message with the corresponding value from the keystream, adding 26 if the result is less than 1 (modular arithmetic)
 4. convert the resulting numbers into letters. This final string is the message (plaintext).


## Keystream:
This is the heart of the cryptosystem.
Solitaire is a system that uses an ordinary deck of 52 cards plus two jokers.
The wildcards must be different from each other, one will be wildcard A and the other wildcard B.
To initialize the deck, place the deck in the initial configuration (key) in a face-up pile.
From then on, follow the following algorithm to generate the keystream:

### 1. 
find joker A and move it down one card (ie swap it with the next card). If joker A is the last card, move it to second position (treat the pile of cards as a loop).

### 2. 
find joker B and move it two cards down. If joker B is the second to last card, move it to the second position. If joker B is the last card, move it to third position.
Examples:
Initial setup after steps 1 and 2:
(beginning) 3 7 A 8 2 ... 9 B 4 1 10 (end) | (beginning) 3 7 8 A 2 ... 9 4 1 B 10 (end)
(beginning) A 3 7 8 2 ... 9 4 1 B 10 (end) | (beginning) 3 B A 7 8 2 ... 9 4 1 10 (end)
(start) 3 7 8 2 ... 9 4 B 1 10 A (end) | (beginning) 3 A 7 8 2 ... 9 4 1 10 B (end)

### 3. 
perform a triple cut, that is, exchange the cards before the first joker with those after the second joker (disregard A and B for this step, first is the closest to the beginning of the deck and second is the closest to the end) . Jokers and the cards between them are not moved. If there are no cards in any of the sections (before or after), treat that section as empty and move this empty one. A deck where jokers are at the ends will not be changed in this step.
Example:
Initial setup after step 3:
(start) 3 7 B 8 2 ... 9 A 4 1 10 (end) | (start) 4 1 10 B 2 ... 9 A 3 7 (end)

### 4. 
perform a counted cut: look at the bottom card, convert it into a number from 1 to 52 (clubs is worth 1, 2 of clubs is worth 2, ..., king of clubs is worth 13, ace of diamonds is worth 14, 2 of diamonds is worth 15, ..., king of diamonds is worth 26, ace of hearts is worth 27, ..., king of hearts is worth 39, ace of spades is worth 40, ..., king of spades is worth 52) . Both jokers are worth 53. Count the value of the last card from the beginning. Cut the deck at the position found. The last card is not moved. If the last card is a joker, the cut will not change the deck. Remember not to change the order of cards in each section.
Example:
Initial setting
(start) 3 7 B 8 2 ... 9 A 1 10 4 (end)
After cutting:
(beginning) 2 ... 9 A 1 10 3 7 B 8 4 (end)

### 5. 
find the out card: look at the top card, and convert it to a value using the same rule as in step 4. Using this value, count from the top of the deck. Write down the value of the card following the last one counted, this is the output. Do not move any cards in this step. If the output is a wildcard, do not write anything down and go back to step 1.
Example:
current configuration
(beginning) 2 6 1 12 7... 9 A 13 10 3 B 8 4 (end)
Output: 1

### 6. 
convert the output to a value (in this conversion, ace of clubs is worth 1, ..., king of clubs is worth 13, ace of diamonds is worth 14, ..., king of diamonds is worth 26, ace of hearts is worth 1, ..., king of hearts is worth 13, ace of spades is worth 14, ..., king of spades is worth 26)

### NOTE:
Transmitter and Receiver must use the same initial configuration of the deck (key), therefore the same must be known by both in advance.
