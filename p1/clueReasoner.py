'''ClueReasoner.py - project skeleton for a propositional reasoner
for the game of Clue.  Unimplemented portions have the comment "TO
BE IMPLEMENTED AS AN EXERCISE".  The reasoner does not include
knowledge of how many cards each player holds.
Originally by Todd Neller
Ported to Python by Dave Musicant

Copyright (C) 2008 Dave Musicant

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Information about the GNU General Public License is available online at:
  http://www.gnu.org/licenses/
To receive a copy of the GNU General Public License, write to the Free
Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.'''

import SATSolver

# Initialize important variables
caseFile = "cf"
players = ["sc", "mu", "wh", "gr", "pe", "pl"]
extendedPlayers = players + [caseFile]
suspects = ["mu", "pl", "gr", "pe", "sc", "wh"]
weapons = ["kn", "ca", "re", "ro", "pi", "wr"]
rooms = ["ha", "lo", "di", "ki", "ba", "co", "bi", "li", "st"]
cards = suspects + weapons + rooms

def getPairNumFromNames(player, card):
    return getPairNumFromPositions(extendedPlayers.index(player),
                                   cards.index(card))

def getPairNumFromPositions(player, card):
    return player*len(cards) + card + 1

# TO BE IMPLEMENTED AS AN EXERCISE
def initialClauses():
    clauses = []

    # Each card is in at least one place (including case file).
    for c in cards:
        clauses.append([getPairNumFromNames(p, c) for p in extendedPlayers])

    # A card cannot be in two places.
    # We want (A + B)(-A + -B) for all A, B in extendedPlayers
    for c in cards:
        for p in extendedPlayers:
            for p2 in extendedPlayers:
                if p != p2:
                    clause1 = []
                    clause2 = []
                    clause1.append(getPairNumFromNames(p, c))
                    clause1.append(getPairNumFromNames(p2, c))
                    clause2.append(-1 * getPairNumFromNames(p, c))
                    clause2.append(-1 * getPairNumFromNames(p2, c))
                    clauses.append(clause1)
                    clauses.append(clause2)

    # At least one card of each category is in the case file.
    clauses.append([getPairNumFromNames(caseFile, s) for s in suspects])
    clauses.append([getPairNumFromNames(caseFile, s) for w in weapons])
    clauses.append([getPairNumFromNames(caseFile, s) for r in rooms])

    # No two cards in each category can both be in the case file.
    for s in suspects:
        for s2 in suspects:
            if s != s2:
                clause1 = []
                clause2 = []
                clause1.append(getPairNumFromNames(caseFile, s))
                clause1.append(getPairNumFromNames(caseFile, s2))
                clause2.append(-1 * getPairNumFromNames(caseFile, s))
                clause2.append(-1 * getPairNumFromNames(caseFile, s2))
                clauses.append(clause1)
                clauses.append(clause2)

    for w in weapons:
        for w2 in weapons:
            if w != w2:
                clause1 = []
                clause2 = []
                clause1.append(getPairNumFromNames(caseFile, w))
                clause1.append(getPairNumFromNames(caseFile, w2))
                clause2.append(-1 * getPairNumFromNames(caseFile, w))
                clause2.append(-1 * getPairNumFromNames(caseFile, w2))
                clauses.append(clause1)
                clauses.append(clause2)

    for r in rooms:
        for r2 in rooms:
            if r != r2:
                clause1 = []
                clause2 = []
                clause1.append(getPairNumFromNames(caseFile, r))
                clause1.append(getPairNumFromNames(caseFile, r2))
                clause2.append(-1 * getPairNumFromNames(caseFile, r))
                clause2.append(-1 * getPairNumFromNames(caseFile, r2))
                clauses.append(clause1)
                clauses.append(clause2)

    return clauses

# TO BE IMPLEMENTED AS AN EXERCISE
def hand(player, cards):
    clauses = []
    for c in cards:
        clauses.append(getPairNumFromNames(player, c))

    return clauses


# TO BE IMPLEMENTED AS AN EXERCISE
def suggest(suggester, card1, card2, card3, refuter, cardShown):
    clauses = []
    player = extendedPlayers[(extendedPlayers.index(suggester) + 1) % 
                             len(extendedPlayers)]

    # If no one refutes, then no one but the suggester and the caseFile
    # could possibly have the card.
    if refuter == None:
        for p in players:
            if p != suggester:
                clauses.append(-1 * getPairNumFromNames(p, card1))
                clauses.append(-1 * getPairNumFromNames(p, card2))
                clauses.append(-1 * getPairNumFromNames(p, card3))
    else:
        while True:
            # If the player refutes, s/he must have at least one card.
            if player == refuter:
                # If we know the card, we can just append it to the clauses.
                if cardShown:
                    clauses.append(getPairNumFromNames(player, cardShown))
                # Otherwise s/he can own at least one of the cards.
                else:
                    clauses.append([getPairNumFromNames(player, card1),
                                 getPairNumFromNames(player, card2),
                                 getPairNumFromNames(player, card3)])
                break
            # We ignore the case file in this situation.
            elif player == caseFile:
                player = extendedPlayers[(extendedPlayers.index(player) + 1) % 
                                         len(extendedPlayers)]
                continue
            # As we pass a player, they cannot have any of the cards because
            # they didn't refute anything.
            else:
                clauses.append(-1 * getPairNumFromNames(player, card1))
                clauses.append(-1 * getPairNumFromNames(player, card2))
                clauses.append(-1 * getPairNumFromNames(player, card3))

                player = extendedPlayers[(extendedPlayers.index(player) + 1) % 
                                         len(extendedPlayers)]

    return clauses

# TO BE IMPLEMENTED AS AN EXERCISE
def accuse(accuser, card1, card2, card3, isCorrect):
    clauses = []
    if isCorrect:
        clauses.append(getPairNumFromNames(caseFile, card1))
        clauses.append(getPairNumFromNames(caseFile, card2))
        clauses.append(getPairNumFromNames(caseFile, card3))
    else:
        clauses.append([-1 * getPairNumFromNames(caseFile, card1),
                     -1 * getPairNumFromNames(caseFile, card2),
                     -1 * getPairNumFromNames(caseFile, card3)])

    return clauses

def query(player, card, clauses):
    return SATSolver.testLiteral(getPairNumFromNames(player, card), clauses)

def queryString(returnCode):
    if returnCode == True:
        return 'Y'
    elif returnCode == False:
        return 'N'
    else:
        return '-'

def printNotepad(clauses):
    for player in players:
        print '\t', player,
    print '\t', caseFile
    for card in cards:
        print card, '\t',
        for player in players:
            print queryString(query(player, card, clauses)), '\t',
        print queryString(query(caseFile, card, clauses))

def playClue():
    clauses = initialClauses()
    clauses.extend(hand("sc", ["wh", "li", "st"]))
    clauses.extend(suggest("sc", "sc", "ro", "lo", "mu", "sc"))
    clauses.extend(suggest("mu", "pe", "pi", "di", "pe", None))
    clauses.extend(suggest("wh", "mu", "re", "ba", "pe", None))
    clauses.extend(suggest("gr", "wh", "kn", "ba", "pl", None))
    clauses.extend(suggest("pe", "gr", "ca", "di", "wh", None))
    clauses.extend(suggest("pl", "wh", "wr", "st", "sc", "wh"))
    clauses.extend(suggest("sc", "pl", "ro", "co", "mu", "pl"))
    clauses.extend(suggest("mu", "pe", "ro", "ba", "wh", None))
    clauses.extend(suggest("wh", "mu", "ca", "st", "gr", None))
    clauses.extend(suggest("gr", "pe", "kn", "di", "pe", None))
    clauses.extend(suggest("pe", "mu", "pi", "di", "pl", None))
    clauses.extend(suggest("pl", "gr", "kn", "co", "wh", None))
    clauses.extend(suggest("sc", "pe", "kn", "lo", "mu", "lo"))
    clauses.extend(suggest("mu", "pe", "kn", "di", "wh", None))
    clauses.extend(suggest("wh", "pe", "wr", "ha", "gr", None))
    clauses.extend(suggest("gr", "wh", "pi", "co", "pl", None))
    clauses.extend(suggest("pe", "sc", "pi", "ha", "mu", None))
    clauses.extend(suggest("pl", "pe", "pi", "ba", None, None))
    clauses.extend(suggest("sc", "wh", "pi", "ha", "pe", "ha"))
    clauses.extend(suggest("wh", "pe", "pi", "ha", "pe", None))
    clauses.extend(suggest("pe", "pe", "pi", "ha", None, None))
    clauses.extend(suggest("sc", "gr", "pi", "st", "wh", "gr"))
    clauses.extend(suggest("mu", "pe", "pi", "ba", "pl", None))
    clauses.extend(suggest("wh", "pe", "pi", "st", "sc", "st"))
    clauses.extend(suggest("gr", "wh", "pi", "st", "sc", "wh"))
    clauses.extend(suggest("pe", "wh", "pi", "st", "sc", "wh"))
    clauses.extend(suggest("pl", "pe", "pi", "ki", "gr", None))
    print 'Before accusation: should show a single solution.'
    printNotepad(clauses)
    print
    clauses.extend(accuse("sc", "pe", "pi", "bi", True))
    print 'After accusation: if consistent, output should remain unchanged.'
    printNotepad(clauses)

if __name__ == '__main__':
    playClue()
