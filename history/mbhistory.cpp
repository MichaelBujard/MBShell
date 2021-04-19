// the history list is an array of history entries. A history entry is declared as follows,
// according to the manual "man history":

typedef void * histdata_t;

typedef struct _hist_entry {
    char *line;
    char *timestamp;
    histdata_t data;
} HIST_ENTRY;

// the history list itself can be declared as

HIST_ENTRY ** the_history_list;

// the state of the History library is encapsulated into a single structure:

/*
 * A structure used to pass around the current state of the history,
 * according to "man history" manual 
 */
typedef struct _hist_state {
    HIST_ENTRY **entries; /* Pointer to the entries themselves */
    int offset;           /* location pointer within this array. */
    int length;           /* Number of elements within this array. */
    int size;             /* Number of slots allocated to this array. */
    int flags;            /* Study TODO: What are these? */
} HISTORY_STATE;

/*
This is broken code. It just lists some history variables.
TODO: 
 -Decide Where do the variables go?
 -Decide which variables do we want to use?
*/


int history_base;
/*
The logical offset of the first entry in the history list.
*/


int history_length;
/*
The number of entries currently stored in the history list.
*/



int history_max_entries;
/*
The maximum number of history entries.  This must be changed using stifle_history().
*/


int history_wite_timestamps;
/*
If non-zero, timestamps are written to the history file, so they can be preserved between sessions.   The  default
       value  is  0,  meaning that timestamps are not saved.  The current timestamp format uses the value of history_com-
       ment_char to delimit timestamp entries in the history file.  If that variable does not have a value (the default),
       timestamps will not be written.
*/


char history_expansion_char;
/*
The character that introduces a history event.  The default is !.  Setting this to 0 inhibits history expansion.
*/

char history_subst_char;
/*
The character that invokes word substitution if found at the start of a line.  The default is ^.
*/

char history_comment_char;
/*
During  tokenization, if this character is seen as the first character of a word, then it and all subsequent char-
       acters up to a newline are ignored, suppressing history expansion for the remainder of the line.  This is disabled
       by default.
*/


char * history_word_delimiters;
/*
The characters that separate tokens for history_tokenize().  The default value is " \t\n()<>;&|".
*/

char * history_no_expand_chars;
/*
The list of characters which inhibit history expansion if found immediately following history_expansion_char.  The
       default is space, tab, newline, \r, and =.
*/


char * history_search_delimiter_chars;
/*
The list of additional characters which can delimit a history search string, in addition to space, tab, : and ? in
       the case of a substring search.  The default is empty.
*/

int history_quotes_inhibit_expansion;
//If  non-zero, double-quoted words are not scanned for the history expansion character or the history comment char-
//       acter.  The default value is 0.

rl_linebuf_func_t * history_inhibit_expansion_function;
/*
 This should be set to the address of a function that takes two arguments: a char * (string) and an int index  into
       that  string  (i).  It should return a non-zero value if the history expansion starting at string[i] should not be
       performed; zero if the expansion should be done.  It is intended for use by applications like bash  that  use  the
       history expansion character for additional purposes.  By default, this variable is set to NULL.
*/