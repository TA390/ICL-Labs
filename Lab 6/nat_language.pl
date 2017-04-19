% The Lexicon:

article([the]).
article([a]).
article([an]).

noun([grass]).
noun([cow]).
noun([girl]).
noun([boy]).
noun([apple]).
noun([song]).

verb([eats]).
verb([chews]).
verb([kicks]).
verb([sings]).

adverb([slowly]).  
adverb([deliberately]).  
adverb([merrily]).  
adverb([sweetly]).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 1 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% create a sentence combining a noun phrase and a verb phrase

sentence(S) :- 
    noun_phrase(NP),
    verb_phrase(VP),
    append(NP, VP, S).


% create a noun phrase. A noun phrase consists of an article followed by a
% noun

noun_phrase(NP) :-
    article(Article),
    noun(Noun),
    append(Article, Noun, NP).


% create a verb phrase (case 1). A verb phrase can be a single verb
% defined as a lexicon above

verb_phrase(Verb) :-
    verb(Verb).

% create a verb phrase (case 2). A verb phrase consists of a verb
% followed by a noun phrase

verb_phrase(VP) :-
    verb(Verb),
    noun_phrase(NP),
    append(Verb, NP, VP).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 2 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% create a noun phrase better'. A valid noun phrase (better) is an article
% followed by a noun. A noun starting with a vowel must be preceded by 'the'
% or 'an'. Nouns that do not start with a vowel must be preceded by 'the' or
% 'a'

noun_phrase_better(NPB) :-
    article(Article),
    noun([Noun|_]),

    % extract the first letter from 'Noun'
    atom_chars(Noun, [First_letter|_]),

    % test that the article and the first letter of the noun form a valid
    % noun phrase (better)
    vowel_char(First_letter, Article),

    % combine the article with the noun to create the noun phrase (better)
    append(Article, [Noun], NPB).


% helper function to test the article against the first letter of the noun
% to ensure they form a valid noun phrase
vowel_char(Letter, Article) :-
	vowel([Letter]) -> \+vowel_article(Article) ;
	                   \+non_vowel_article(Article).


% facts to state which article does not bind with a noun depending on what
% its first character is e.g. a noun staring with the vowel 'a' does not
% bind with vowel_article.
non_vowel_article([an]).
vowel_article([a]).

% vowel facts
vowel([a]).
vowel([e]).
vowel([i]).
vowel([o]).
vowel([u]).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 3 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% definition for cadvs, to create a single element adverb list

cadvs(CAdverbs) :-
	adverb(CAdverbs).

% definition for cadvs, to create a list of adverbs containing no
% duplicates. The adverbs are separated by commas which are themselves
% elements of the list. The final two adverbs are separated by 'and'

cadvs(CAdverbs) :-
	get_list([], List),
	add_separators(List, CAdverbs).


% base case - two equal lists. the second argument is instantiated to match
% the first argument for all combinations of adverbs.
get_list(List, List).

% create a list of adverbs (all combinations), checking that each list does
% not contain any duplicates.
get_list(List, ListCpy) :-
	adverb([Adverb]),
	append(List,[Adverb], Temp),
	\+member(Adverb, List),
	get_list(Temp, ListCpy).


% base case - a list of two elements. The second argument instantiates
% itself to the first, inserting the word 'and' inbetween the two elements
add_separators([Word1,Word2], [Word1,and,Word2]).

% recursively add a comma to the second parameter between the head and tail
% The second parameter is the same list with commas between each word. The
% base case places an 'and' between the last two words of the list.
add_separators([Word|Sentence], [Word,Comma|Sentence2]) :-
	comma(Comma),
	add_separators(Sentence, Sentence2).


% fact for a comma
comma(',').




% verb_phrase_two is a helper function used by verb_phrase_better. The
% function creates a list containing a single verb
verb_phrase_two(VP2) :-
	verb(VP2).

% verb_phrase_two is a second helper function used by verb_phrase_better.
% The function creates a list containing a verb, followed by a noun phrase
% as defined by noun phrase better
verb_phrase_two(VP2) :-
    verb(V),
    noun_phrase_better(NP),
    append(V, NP, VP2).


% verb_phrase_better (case 1) creates a verb phrase which is formed of a
% verb, followed by a noun phrase. The function calls verb_phrase_two to
% combine the verb and the noun phrase (better)
verb_phrase_better(VPB) :-
	verb_phrase_two(VPB).

% verb_phrase_better (case 2) creates a verb phrase which is formed of a
% conjunction of adverbs followed by a verb, followed by a noun phrase
% (better). The function calls verb_phrase_two to combine the verb and the
% noun phrase (better)
verb_phrase_better(VPB) :-
	cadvs(CA),
	verb_phrase_two(VP),
	append(CA, VP, VPB).


% sentence_better combines the improved vocabularly created by
% noun_phrase_better and verb_phrase_better. The function combines the two
% to create a sentence (better)
sentence_better(SB) :- 
    noun_phrase_better(NPB),
    verb_phrase_better(VPB),
    append(NPB, VPB, SB).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 4 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% actions finds all verbs in the list 'Text' which describe what the 'Actor'
% is doing. The results make up the list of actions 'As'.
actions(Actor, Text, As) :-
    noun([Actor]),
    findall(Action, find_action(Actor, Text, Action), As).


% find_action looks for pairs in the list 'Text' which have an 'Actor'
% (noun) followed by 'Action' (verb).
find_action(Actor, Text, Action) :-
    append(_,[Actor,Action|_],Text),
    verb([Action]).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 5 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% create a list of pairs where the first element is the action (verb)
% performed by the 'Actor' (noun) and the second element is a list of
% adverbs describing the action. An empty list is created if no adverbs are
% found. The list 'As' is created from the list 'Text'
actions_and_adverbs(Actor, Text, As) :-
	noun([Actor]),
	findall(Adverbs, get_adverbs(Actor, Text, Adverbs), As).


% base case - when the head of the list (middle parameter) is equal to the
% 'Actor' (first parameter), find the sublist between the 'Actor' and the
% next verb 'V'. The result is the the third parameter which is a pair
% (verb, list of adverbs)
get_adverbs(Actor, [Actor|T], (V,As)) :-
	verb([V]),
	append([Actor|List], [V|_], [Actor|T]),
	remove_conjunction(List, As),
	% check that the list created is a valid combination of adverbs
	get_list([],As).

% recursively break down the list (middle parameter) until the base case
% is satisfied (or no case)
get_adverbs(Actor, [_|T], As) :-
	get_adverbs(Actor, T, As).


% base case (case 1) - an empty list, for when no adverbs are found
remove_conjunction([], []).

% base case (case 2) - when the first list contains three elements,
% instantiate the second list to equal the first, but without the
% conjunction 
remove_conjunction([Word1,and,Word2], [Word1,Word2]).

% remove_conjunction recursively breaks down the first list adding the
% head of the list to the second list. The base case instantiates
% the tail of the second list to either an empty list or a list
% without the conjunction
remove_conjunction([H|T], [H|As]) :-
	remove_conjunction(T, As).