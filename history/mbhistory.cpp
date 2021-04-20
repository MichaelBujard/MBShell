// the history list is an array of history entries. A history entry is declared as follows,
// according to the manual "man history":

typedef void * histdata_t;

typedef struct _hist_entry {
    char *line;
    char *timestamp;
    histdata_t data;
} HIST_ENTRY;

// the history list itself can be declared as
HIST_ENTRY ** mb_history_list;
/*
 * This function returns info about the entire history list/individual list entries.
 * The above mb_history_list (void) return a NULL terminated array of HIST_ENTRY * that is the current
 * input history. Element 0 of this list is the beginning of time. 
 * 
 * If there is no history, return null.
 */

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
    int flags;            /* Study TODO: What are these? "If the flags member includes
                           * HS_STIFLED, then the history has been stifled. 
                           * What does that mean, really?
                           */
} HISTORY_STATE;

/*
 * TODO: Write functions that use these data structures.
 * History Functions SECTION:
 * This section describes the calling sequence for the various functions exported by the GNU History 
 * library.
 * e.g. void using_history (void) :
 * Begin a session in which the history functions might be used. This initializes the interactive 
 * variables.
 *
 * e.g. 2: 
 * HISTORY_STATE * history_get_history_state (void)
 * Returns a structure describing the current state of the input history.
 *
 * e.g. 3: void history_set_history_state (HISTORY_STATE *state)
 * Set the state of the history list according to state.
 *
 */

/*
 * History List Management SECTION:
 * These functions manage individual entries on the history list, / set params managing list itself.
 */
 /*
  * void add_history(const char *string)
  * Place string at the end of the history list. The associated data field (if any) is set to NULL.
  */

/*
 * Searching the History List SECTION:
 * Functions that allow searching of the history list for entries containing a specific string. 
 * Searching may be performed both forward and backward from the current history posn.
 * The search may be ANCHORED
 *
 * FYI, ANCHORED means "the string must match at the beginning of the history entry"
 * 
 * e.g. 1 
 * int history_search (const char *string, int direction)
 * description of function: Search the history for _string_, starting at the current history offset.
 * If direction is less than 0, then the search is through previous entries, otherwise through 
 * subsequent entries. If string is found, then the current history index is set to that history entry,
 * and the value returned is the offset in the line of the entry where _string_ was found.
 * Otherwise, nothing is changed,  and a -1 is returned.
 */

 /*
  * int history_search_prefix (const char * string, int direction)
  *
  * Description: Search the history for _string_, starting at the current history offset.
  * The search is anchored:
  *   - i.e. matching lines must begin with _string_.
  * If direction is less than 0, then the search is through previous entries, otherwise 
  * through subsequent entries. 
  * If _string_ is found, then the current history index is set to that history entry, and the value 
  * returned is 0. Otherwise, nothing is 
  * changed, and a -1 is returned.
  */

/*
 * history_search_pos (const char *string, int direction, int pos)
 * Search  for  string  in the history list, starting at pos, an absolute index into the list. 
 * If direction is negative, the search proceeds backward from pos, otherwise forward.  
 * Returns the absolute index of the history element where string was found, or -1 otherwise.
 */
int history_search_pos(const char *string, int direction, int pos) {
    return 0;
}

/*
 * Managing the History File SECTION:
 * The History library can read the history from and write it to a file.
 * This section of code (eventually) will contain functions + documentation for 
 * managing a history file...
 */

 /*
  * int read_history(const char *filename)
  * Add the contents of filename to the history list, a line at a time.
  * If filename is NULL, then read  from  ~/.history.
  * Returns 0 if successful, or errno if not.
  */
int read_history(const char *filename) {
    return 0;
}

/*
 * int read_history_range(const char *filename, int from, int to)
 * description: Read a range of lines from filename, adding them to the history list.
 * Start reading at line from and end at to.
 * If from is zero, start at the beginning.  
 * If to is less than from, then read until the end of the file.  If  filename is NULL, 
 * then read from ~/.history.  Returns 0 if successful, or errno if not.
 */
int read_history_range(const char *filename, int from, int to) {
    return 0;
}

/*
 * int write_history (const char *filename)
 * Description: 
 * Write  the  current  history  to filename, overwriting filename if necessary.
 * If filename is NULL, then write the
 * history list to ~/.history.  Returns 0 on success, or errno on a read or write error.
 */
int write_history (const char *filename)

/*
 * int append_history (int nelements, const char *filename)
 * Append the last nelements of the history list to filename.
 * If filename is NULL, then append to  ~/.history.
 * Returns 0 on success, or errno on a read or write error.
 *
 */
int append_history (int nelements, const char *filename) {
    return 0; // success!
}


/*
 * int history_truncate_file (const char *filename, int nlines)
 * Truncate  the  history file filename, leaving only the last nlines lines.
 * If filename is NULL, then ~/.history is truncated.
 * Returns 0 on success, or errno on failure.
 */
int history_truncate_file (const char *filename, int nlines) {
    return 0; // Success!
}

/*
 * History Expansion SECTION:
 * do we want / need to implement this functionality?
 */

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