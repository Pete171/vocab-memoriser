# Vocab Memoriser

A small tool for drilling vocabulary for a foreign language into your short term memory.

This is only intended for personal use on a Linux machine.

## Details

* Vocabulary that is to be drilled is entered into a text file called `words.txt`.
* There must only be once space on each line of this file: this line separates the question from the answer.
* See sample words file:

```
question_word1 answer_word1
question_word2 answer_word2
```

* The application runs in an infinite loop.
* Each iteration of the loop calculates which words are 'ready for review' and displays them in a random order.
* For each word in an iteration the 'question word' is displayed and is followed - one second later - by the 'answer word'.
* The user is prompted to enter '1' if they remembered the word or '0' if they didn't.

* In the first iteration of the loop all words are 'ready for review'.
* If the word was correct then the word is then marked as 'ready for review' in 1 second.
* In the next iteration it will be ready in 2 seconds.  Then in the next iteration in 4 seconds, then 8, 16, etc.  ( so each iteration increases the time before the word is shown again in multipliers of 2).
* If a word is ever wrong (the user entered 0) the ready for review' time is reset to 1 and the time begins again increasing multiplicatively.
* If no words are ready for review then text is shown to distract the user from the words they were reviewing.  It is suggested this is text from the target language the user is trying to learn.  Just populate `distraction.txt` with whatever you want.
* An eample `words.txt` and `distraction.txt` have been provided with the source so this can be instantly run.
* Can run with a param '1' or '0'.  1 will reverse the order of the words so that the native language is shown as the question and the target language is the answer.  0 is the default.
## To compile:

1.  Checkout this repository into a directory on your local machine.
2.  Run make

## To run:

```bash
  `./main
````
