# Vocab Memoriser

A small tool for drilling vocabulary for a foreign language into your short term memory.

This is only intended for personal use on a Linux machine.

Vocabulary that is to be derilled is entered into a text file called 'words.txt'.  There must only be once space on each line: this line separates the question from the answer.  See sample words file:

question_word1 answer_word1
question_word2 answer_word2

The application runs in an infinite loop.  Each iteration of the loop calculates which words are ready for review and displays them in a random order.  The 'question word' is displayed and then one second later the 'answer word' is displayed.  The user is prompted to enter '1' if they remembered the word or '0' if they didn't.

In the first iteration of the loop all words are ready for review.  If the word was correct then the word will then be set to be ready for review in 1 seecond.  When it is next seen it will be shown in 2 seconds, then 4, then 8, etc.

If a word is wrong (you entered 0)  it is reset to be ready for review in 1 second and the ready for review time begins multiplicatively increasing to 2, 4, 8, etc. as before.

If no words are ready for review then text is shown to distract the user from the words they were reviewing.  The text displayed can be anything - just populate distraction.txt with what you want.

To compile:
1 - Checkout this repository into a directory on your local machine.
2 - Run make

To run:
  `./main
