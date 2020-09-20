:- module(hw, [cost/2, sort_units/2, buyable/4]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE

costofunit(Name,Cost):- unit(Name,C,_), Cost=C.

cost([],0).
cost([active_unit(Name,Star)|Rem],Cost):- cost(Rem,SCost),
                                        costofunit(Name,C),
                                        Cost is 3^(Star-1)*C+SCost.
                                        


sort_units(List,Sorted):- sort(List,[],ASorting),reverse(ASorting,Sorted).

sort([],Acc,Acc):-!.
sort([Name|Rem],Acc,Sorted):-sorting(Name,Acc,NAcc),sort(Rem,NAcc,Sorted).

sorting(NameL,[NameR|RemR],[NameR|NRem]):- costofunit(NameL,L),costofunit(NameR,R),
                                            L>R,
                                            sorting(NameL,RemR,NRem).
sorting(NameL,[NameR|RemR],[NameL,NameR|RemR]):-costofunit(NameL,L),costofunit(NameR,R),
                                                L=<R.
sorting(NameL,[],[NameL]).

reversing([],L,L):-!.
reversing([H|Rem],T,L):-reversing(Rem,[H|T],L).
reverse(L,LR):-reversing(L,[],LR).


buyable(List,Money,BList,RMoney):-sublist(List,S),
                                S =BList,
                                costoflist(S,C),
                                RMoney is Money-C,
                                RMoney>=0.

sublist([],[]).
sublist([H|Rem],[H|SRem]):-sublist(Rem,SRem).
sublist([_|Rem],SRem):-sublist(Rem,SRem).


 costoflist([],0).
 costoflist([Name|Rem],Cost):-costoflist(Rem,Scost),costofunit(Name,C),Cost is C+Scost.