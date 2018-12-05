import pandas as pd
import re
dfext = pd.read_csv('/Users/vkumar/VIEW/R_Working/X/Health-Tweets/cbchealth.txt', sep='|', error_bad_lines=False, header=None)
dfext.head(3)

#Display column 2, row 1
dfext.loc[1,2]
'''
'Sabra hummus recalled in U.S. http://www.cbc.ca/news/health/sabra-hummus-recalled-in-u-s-1.3026865?cmp=rss'
We want everything before 'http' for each row
we test our idea of the correct code and store that on a temps dataframe, then if we make mistakes we don't affect 
the data As a rule of thumb it is a good idea to define a 'reference' data structure when testing code, and work on copies.
'''

te = dfext[dfext.columns[2]].str.split('http', expand=True)
te.head()
#nearly there, we can check te is a pandas object, if yes select just the first column

type(te)
te = dfext[dfext.columns[2]].str.split('http', expand=True)[0]
te.head(3)

'''
0    Drugs need careful monitoring for expiry dates...
1                       Sabra hummus recalled in U.S. 
2    U.S. sperm bank sued by Canadian couple didn't...
Name: 0, dtype: object
Now we know exactly how to achieve the split, we can operate on the reference data frame. 
We will add an extra column named 'plain_text' to our df where we store our wrangling. We can also delete te to free space
'''
dfext["plain_text"] = dfext[dfext.columns[2]].str.split('http', expand=True)[0]
dfext["plain_text"].head(1)

'''
0    Drugs need careful monitoring for expiry dates...
Name: plain_text, dtype: object
We want to count word occurence, so we are getting rid of the punctuation, which would be counted as words otherwise. 
Some libraries such as nltk would do that, but for code maintenance reasons we might want to define our own function 
using as few libraries as possible.
We store the result in another column of our panda df, named 'out'
'''


def clean(text):
    unwanted_char = re.compile(r'(\\r|\\n)+')
    if unwanted_char.match(text):
        clean_text = text.replace(unwanted_char,' ')
    else:
        clean_text = text
    return clean_text

dfext["out"] = dfext['plain_text'].str.replace('[^\w\s]','').apply(clean)
dfext['out']

'''
0       Drugs need careful monitoring for expiry dates pharmacists say                      
1       Sabra hummus recalled in US                                                         
2       US sperm bank sued by Canadian couple didnt verify donor info executive says        
3       Manitoba pharmacists want clampdown on Tylenol 1 sales                              
4       Mom of 7 spooked by vaccinations reverses stand  but then kids get sick             
5       Hamilton police send mental health pros to the front lines with cops                
6       Wind turbine noise linked to only 1 health issue  annoyance                         
7       Wild West of ecigarettes sparks debate over regulation                              
8       Dementia patients sold unproven brainwave optimization        
'''

#Now we define a lambda function to split every row into words, and we apply it to the data frame with the apply method
dfext['out'].apply(lambda x : len(x.split()))
dfext['all_words_count'] = dfext['out'].apply(lambda x : len(x.split()))
dfext['all_words_count']

'''
to find word occurence, better define an intermediate step : 
transform words as tokens, so that the count is not done on character but on a specific word. 
Python NLP libraries have plenty of methods for doing just this.
'''
#one example with nltk, found on the web
import nltk
from nltk import FreqDist

texts = 'hi there hello there'
words = nltk.tokenize.word_tokenize(texts) #nltk.word_tokenize(texts)
print(words)
fdist = FreqDist(words)
print(fdist)

from sklearn.feature_extraction.text import CountVectorizer

vec = CountVectorizer()
X = vec.fit_transform(dfext['out'])
df = pd.DataFrame(X.toarray(), columns=vec.get_feature_names()) #returns a sparse matrix
print(df)

# an example with scikit-learn, found on the web
import numpy as np
from sklearn.feature_extraction.text import CountVectorizer
vectorizer = CountVectorizer()
X = vectorizer.fit_transform(dfext['out'])
freq = np.ravel(X.sum(axis=0)) # sum each columns to get total counts for each word
#print(freq)
import operator
# get vocabulary keys, sorted by value
vocab = [v[0] for v in sorted(vectorizer.vocabulary_.items(), key=operator.itemgetter(1))]
print(vocab)

fdist = dict(zip(vocab, freq)) # return same format as nltk
print(fdist)

'''
These examples do the job but the output format can be very unconfortable. 
Sometimes we just don't know the correct libraries, but when the outputs are that unusables, 
it should be a warning, either we use the library the wrong way or there must be a more convenient one.
'''
from nltk.tokenize import word_tokenize
dfext['token'] = dfext['out'].apply(word_tokenize)
dfext['token'].head(4)

'''
0    [Drugs, need, careful, monitoring, for, expiry...
1                    [Sabra, hummus, recalled, in, US]
2    [US, sperm, bank, sued, by, Canadian, couple, ...
3    [Manitoba, pharmacists, want, clampdown, on, T...
Name: token, dtype: object
'''

#for the record, nltk has tweet dedicated methods
from nltk.tokenize import TweetTokenizer
twt = TweetTokenizer()
(dfext['out'].apply(twt.tokenize)).head()

'''
0    [Drugs, need, careful, monitoring, for, expiry...
1                    [Sabra, hummus, recalled, in, US]
2    [US, sperm, bank, sued, by, Canadian, couple, ...
3    [Manitoba, pharmacists, want, clampdown, on, T...
4    [Mom, of, 7, spooked, by, vaccinations, revers...
Name: out, dtype: object
'''
#We can also define our own function for counting tokens
def count_word_occurrence(tokens):
    dico = {}
    for tok in tokens:
        if tok not in dico:
            dico[tok]=1
        else:
            dico[tok]+=1
    return dico

dfext['word_occ'] = dfext['token'].apply(lambda x: count_word_occurrence(x))
print(dfext['word_occ'].head(5))
'''
0    {'Drugs': 1, 'need': 1, 'careful': 1, 'monitor...
1    {'Sabra': 1, 'hummus': 1, 'recalled': 1, 'in':...
2    {'US': 1, 'sperm': 1, 'bank': 1, 'sued': 1, 'b...
3    {'Manitoba': 1, 'pharmacists': 1, 'want': 1, '...
4    {'Mom': 1, 'of': 1, '7': 1, 'spooked': 1, 'by'...
Name: word_occ, dtype: object
'''
type(dfext['word_occ'])

from IPython.display import display, HTML
de = pd.DataFrame(dfext['word_occ'])

#pd.set_option('display.max_rows', 500)
#pd.set_option('display.max_columns', 500)

pd.set_option('display.max_colwidth', -1)

display(HTML(de.to_html()))
'''
compute cosine similarity between the first two lines:
From Word count occurence define 2 vectors V1 and V2 such that each vectors counts how many times each word appears
in each text in regard of the total set of words
This blog post is really clear on the step by step proc: 
https://stackoverflow.com/questions/1746501/can-someone-give-an-example-of-cosine-similarity-in-a-very-simple-graphical-wa/1750187#1750187
'''

import math
from collections import Counter

def build_vector(iterable1, iterable2):
    counter1 = Counter(iterable1)
    counter2 = Counter(iterable2)
    all_items = set(counter1.keys()).union(set(counter2.keys()))
    vector1 = [counter1[k] for k in all_items]
    vector2 = [counter2[k] for k in all_items]
    return vector1, vector2

def cosim(v1, v2):
    dot_product = sum(n1 * n2 for n1, n2 in zip(v1, v2) )
    magnitude1 = math.sqrt(sum(n ** 2 for n in v1))
    magnitude2 = math.sqrt(sum(n ** 2 for n in v2))
    return dot_product / (magnitude1 * magnitude2)


l1 = "Julie loves me more than Linda loves me".split()
l2 = "Jane likes me more than Julie loves me or".split()


v1, v2 = build_vector(l1, l2)
print(cosim(v1, v2))

'''
Now we need to decide how to encode our text. This is where we need to keep in mind the production 
context of the data and the objective of the analysis.
go back to the source and check avalable informations https://archive.ics.uci.edu/ml/datasets/

From the decision we make we could have to rebuild entirely our reference data structure. i.e, 
if we want to compare bbc tweets with another medias's tweets we would need to build a corpus for each file and then compare them. If we want to analyse a more general semantic space on health for information retrieval, as the authors did, we would build just one big corpus.

https://arxiv.org/abs/1705.00995
'''