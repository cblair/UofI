uint16 sq_root (uint32 vsqn) {
    bits32 vsq = vsqn;
    bits32 asq = 0;
    bits16 a = 0;

    uint16 v =
	for uint4 i in [16] {
	    uint4 n = 15-i;
	    bits16 sa = a<<1;
	    bits32 sasq = asq<<2;
	    bits32 nasq = (uint32)sasq + (uint16)((sa<<1) | 0b1);
	    next a, next asq =
	        if (nasq <= (vsq>>((uint5)n*2))) return (sa|0b1, nasq)
		else                      return (    sa, sasq);
	    } return (final (a));
    } return (v);

uint16[:] main (uint32 A[:]) {
    uint16 R[:] =
        for a in A
	    return (array (sq_root (a)));
    } return (R);
