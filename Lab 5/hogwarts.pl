% the students in Hogwarts
student(hp, 'Harry James Potter', gryffindor).
student(hg, 'Hermione Jean Granger', gryffindor).
student(rw, 'Ronald Weasley', gryffindor).
student(ll, 'Luna Lovegood', ravenclaw).
student(cc, 'Cho Chang', ravenclaw).
student(tb, 'Terry Boot', ravenclaw).
student(ha, 'Hannah Abbott', hufflepuff).
student(cd, 'Cedric Diggory', hufflepuff).
student(nt, 'Nymphadora Tonks',hufflepuff).
student(dm, 'Draco Malfoy', slytherin).
student(gg, 'Gregory Goyle', slytherin).
student(vc, 'Vincent Crabbe', slytherin).
student(ta, 'Taha Abdalla', slytherin).

% the teachers in Hogwarts
teacher(ad, 'Albus Percival Wulfric Brian Dumbledore').
teacher(ff, 'Filius Flitwick').
teacher(rh, 'Rubeus Hagrid').
teacher(gl, 'Gilderoy Lockhart').
teacher(rl, 'Remus John Lupin').
teacher(mm, 'Minerva McGonagall').
teacher(qq, 'Quirinus Quirrell').
teacher(ss, 'Severus Snape').
teacher(ps, 'Pomona Sprout').
teacher(st, 'Sibyll Patricia Trelawney').
teacher(mh, 'Madam Hooch').
teacher(as, 'Aurora Sinistra').
teacher(cub, 'Cuthbert Binns').
teacher(bb, 'Bathsheba Babbling').
teacher(sv, 'Septima Vector').
teacher(chb, 'Charity Burbage').
teacher(wt, 'Wilkie Twycross').

% compulsory courses for the MSc in Magic
compCourse(astro, 'Astronomy', as).
compCourse(charms, 'Charms', ff).
compCourse(defence, 'Defence against the Dark Arts', qq).
compCourse(fly, 'Flying', mh).
compCourse(herb, 'Herbology', ps).
compCourse(history, 'History of Magic', cub).
compCourse(potions, 'Potions', ss).
compCourse(trans, 'Transfiguration', mm).

% optional courses for the MSc in Magic
optCourse(runes, 'Study of Ancient Runes', bb).
optCourse(arith, 'Arithmancy', sv).
optCourse(muggle, 'Muggle Studies', chb).
optCourse(creatures, 'Care of Magical Creatures', rh).
optCourse(div, 'Divination', st).
optCourse(app, 'Apparition', wt).
optCourse(choir, 'Frog Choir', ff).
optCourse(quid, 'Quidditch', mh).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 2 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Facts corresponding to the options for each student

% gryffindor options
enrolled_opt(hp, runes).
enrolled_opt(hp, muggle).
enrolled_opt(hp, quid).

enrolled_opt(hg, runes).
enrolled_opt(hg, arith).
enrolled_opt(hg, creatures).
enrolled_opt(hg, div).
enrolled_opt(hg, app).
enrolled_opt(hg, choir).

enrolled_opt(rw, muggle).
enrolled_opt(rw, app).
enrolled_opt(rw, choir).

% hufflepuff options
enrolled_opt(ha, arith).
enrolled_opt(ha, choir).
enrolled_opt(ha, quid).

enrolled_opt(cd, app).
enrolled_opt(cd, runes).
enrolled_opt(cd, muggle).

enrolled_opt(nt, div).
enrolled_opt(nt, app).
enrolled_opt(nt, choir).

% ravenclaw options
enrolled_opt(ll, quid).
enrolled_opt(ll, arith).
enrolled_opt(ll, div).

enrolled_opt(cc, choir).
enrolled_opt(cc, creatures).
enrolled_opt(cc, quid).

enrolled_opt(tb, runes).
enrolled_opt(tb, quid).
enrolled_opt(tb, app).

% syltherin options
enrolled_opt(dm, quid).
enrolled_opt(dm, div).
enrolled_opt(dm, muggle).

enrolled_opt(gg, runes).
enrolled_opt(gg, arith).
enrolled_opt(gg, creatures).

enrolled_opt(vc, creatures).
enrolled_opt(vc, quid).
enrolled_opt(vc, muggle).

enrolled_opt(ta, muggle).
enrolled_opt(ta, div).
enrolled_opt(ta, quid).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 3 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% instantiate/unify a students id 'SID' with the short course name 'SCN' of any courses
% they are enrolled in
enrolled(SID, SCN) :-
    enrolled_opt(SID, SCN).
enrolled(SID, SCN) :-
    student(SID, _, _),
    compCourse(SCN, _, _).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 4 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% instantiate/unify a teacher 'TN' to a course they teach 'SCN'
teaches(TN, SCN) :-
    teacher(TID, TN),
    compCourse(SCN, _, TID).
teaches(TN, SCN) :-
    teacher(TID, TN),
    optCourse(SCN, _, TID).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 5 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% instantiate/unify a student 'SN' with a teacher they are taught by 'TN'
taughtBy(SN, TN) :-
    student(SID, SN, _),
    teaches(TN, SCN),
    enrolled(SID, SCN).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 6 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% instantiate/unify a student 'SN' with an optional course they have enrolled in 'CN'
takesOption(SN, CN) :-
    student(SID, SN, _),
    enrolled_opt(SID, SCN),
    optCourse(SCN, CN, _).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 7 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% instantiate/unify a student 'SN' with a list of all optional course they have enrolled in
% 'OptCourses'
takesAllOptions(SN, OptCourse) :-
    optCourseList(SN, List),
    OptCourse = List.

% return a sorted list 'OptList' containing the options of student 'SN'
optCourseList(SN, OptList) :-
    student(_, SN, _),
    setof(SCN, takesOption(SN, SCN), OptList).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 8 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% create a list 'Student' for a given 'House'
studentsInHouse(House, Student) :-
    house(House),
    findall(Members, membersOfHouse(House, Members), MembersList),
    listOfMembers(MembersList, NameOnlyList),
    Student = NameOnlyList.

% facts for each valid house
house(gryffindor).
house(hufflepuff).
house(ravenclaw).
house(slytherin).

% create a list 'Member' of [SID,SN] pairs for each student of 'House'
membersOfHouse(House, Member) :-
    student(SID, SN, House),
    Member = [SID, SN].

% take a list 'HouseMembers' of [SID,SN] pairs and populate 'NamesOnly' with student names
% sorted by their SID value
listOfMembers(HouseMembers, NamesOnly) :-
    sort(HouseMembers, SortedHouseMembers),
    extractSN(SortedHouseMembers, NamesOnly).

% extract student names from a list of [SID,SN] pairs
extractSN([[_, SN]|Tail],[SN|NameList]) :-
    extractSN(Tail, NameList).
% base case
extractSN([], []).
    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 9 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% create a list of four elements 'StudentsByHouse', where each element is a list of the
% students enrolled on the course idetified by 'SCN' and 'CN'
studentsOnCourse(SCN, CN, StudentsByHouse) :-
    coursesByHouse(SCN, CN, StudentsByHouse).

% prefix the House name to each 'HouseList' generated by 'createList'
coursesByHouse(SCN, CN, HouseList) :-
    setof(House-HouseList, createList(SCN, CN, House, HouseList), HouseList).

% create a list for each house with members enrolled on the course 'CN'
createList(SCN, CN, House, List) :-
    courses(SCN, CN),
    house(House),
    findall(Students, convertToNames(SCN, House, Students), List).

% instantiate/unify 'SCN' and 'CN' to a course (optional or compulsary) 
courses(SCN, CN) :-
    (optCourse(SCN, CN, _);
     compCourse(SCN, CN, _)).

% return the 'Name' for a student on the course 'SCN' who is in the house 'House'
convertToNames(SCN, House, Name) :-
    enrolled(SID, SCN),
    student(SID, Name, House).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 10 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% 'SN1' and 'SN2' are the names of two different students who are enrolled on the same
% optional course 'CN', where 'CN' is the course name
sharedCourse(SN1, SN2, CN) :-
    takesOption(SN1, CN),
    takesOption(SN2, CN),
    SN1 \= SN2.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Question 11 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% 'SN1' and 'SN2' are student names. 'Courses' is a list made up of the three courses they
% have in common
sameOptions(SN1, SN2, Courses) :-
    takesAllOptions(SN1, SN1Courses),
    takesAllOptions(SN2, SN2Courses),
    validateOptions(SN1Courses, SN2Courses, Courses),
    SN1 \= SN2.

% test that the list 'Courses' created from the common member of 'List1' and 'List2' has
% three elements, which corresponds to a successful match
validateOptions(List1, List2, Courses) :-
    compareLists(List1, List2, Matched),
    Courses = Matched,
    length(Courses, ListLength),
    3 =< ListLength.

% take two lists, 'List1' and 'List2' and produce a third list 'Match' containing all
% courses found in both 'List1' and 'List2'.
compareLists(List1, List2, Matched) :-
    findall(Element, memberOfTwoLists(Element, List1, List2), Matched).

% find all members that 'List1' and 'List2' have in common
memberOfTwoLists(Element, List1, List2) :-
    member(Element, List1),
    member(Element, List2).
