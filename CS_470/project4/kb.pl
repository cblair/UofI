%(X,Y) - X is a <> of Y

%specific definitions
parent(bob,joe).
parent(alice,joe).
parent(bob,val).
parent(alice,val).
parent(paul,bernard).
parent(jolien,bernard).
parent(bernard,dee).
parent(evalin,dee).
parent(bernard,elsie).
parent(evalin,elsie).
parent(jim,casey).
parent(dee,casey).

%specific definitions
man(joe).
man(bob).
man(jerry).
man(jim).
man(casey).
man(paul).
woman(val).
woman(alice).
woman(elsie).
woman(dee).
woman(jolien).

%non-sex specific statements
child(X,Y):- parent(Y,X).
sibling(X,Y):- parent(Z,X),parent(Z,Y).
married(X,Y)  :- parent(X,Z),parent(Y,Z).
	married(jerry,elsie). %don't have kids

%sex specific statements
father(X,Y)   :- parent(X,Y),man(X).
mother(X,Y)   :- parent(X,Y),woman(X).
sister(X,Y)   :- sibling(X,Y),woman(X).
brother(X,Y)  :- sibling(X,Y),man(X).
aunt(X,Y)     :- (sister(X,Z),parent(Z,Y));(married(X,Z),uncle(Z,Y)).
uncle(X,Y)    :- (brother(X,Z),parent(Z,Y));(married(X,Z),aunt(Z,Y)).

ancestor(X,Y) :- parent(X,Y).
ancestor(X,Y) :- parent(X,Z),ancestor(Z,Y).
decendent(X,Y):- child(X,Y).
decendent(X,Y):- child(X,Z),decendent(Z,Y).
related(X,Y)  :- parent(X,Y).
related(X,Y)  :- parent(X,Z),related(Z,Y).
related(X,Y)  :- child(X,Y).
related(X,Y)  :- child(X,Z),related(Z,Y).
related(X,Y)  :- married(X,Y).
related(X,Y)  :- married(X,Z),related(Z,Y).
