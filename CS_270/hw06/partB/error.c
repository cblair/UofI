#include "sassy.h"
#include "node.h"
#include "error.h"

char *op_str (Node);

int global_error = FALSE;

void handle_error (Error err, ErrLevel level, int line, char *file, char *func, ...)
    {
    va_list ap;
    int v;

    va_start (ap, func);

    global_error = TRUE;

    if (quiet)
	goto leave_error;

    fprintf (stderr, "error (reported from SA-C compiler)");
    if (line > 0)
        fprintf (stderr, ", line %d", line);
    if ((file != NULL) && (strcmp (file, "") != 0))
        fprintf (stderr, ", file '%s'", file);
    if ((func != NULL) && (strcmp (func, "") != 0))
        fprintf (stderr, ", function '%s'", func);
    fprintf (stderr, ":\n");

    switch (err)
	{
	case ErrMallocFailure :
	    fprintf (stderr, "  heap allocation failed\n");
	    break;
	case ErrMainOpenSource :
	    fprintf (stderr, "  can't open SA_C source file\n");
	    break;
	case ErrMainBufSizeError :
	    fprintf (stderr, "  couldn't read the buffer size argument\n");
	    break;
	case ErrMainOpenDdcfDump :
	    fprintf (stderr, "  can't open DDCF dump file for writing\n");
	    break;
	case ErrMainMainDashM :
	    fprintf (stderr, "  -m option cannot have a 'main'\n");
	    break;
	case ErrMainMissingDashM :
	    fprintf (stderr,
	        "  can't find function name '%s' for 'main' substitution\n",
		find_sym (v));
	    break;
	case ErrScanDanglingString :
	    fprintf (stderr, "  dangling string\n");
	    break;
	case ErrScanDanglingComment :
	    fprintf (stderr, "  dangling comment\n");
	    break;
	case ErrScanInvalidChar :
    	    v = va_arg (ap, int);
	    fprintf (stderr, "  invalid character '%c'\n", v);
	    break;
	case ErrScanBitWidth :
	    fprintf (stderr, "  type width exceeds 32 bits\n");
	    break;
	case ErrScanBitWidthFrac :
	    fprintf (stderr, "  fractional size is too large\n");
	    break;
	case ErrScanBitWidthLow :
	    fprintf (stderr, "  type width too small\n");
	    break;
	case ErrParse :
	    fprintf (stderr, "  syntax error\n");
	    break;
	case ErrParseMaskOnStructOp :
	    fprintf (stderr,
		"  mask expression not allowed on loop return operator\n");
	    break;
	case ErrParseArrayDefNeedsSize :
	    fprintf (stderr,
		"  array constant definition needs size(s) in left hand side spec\n");
	    break;
	case ErrParseNoCaseOrDefault :
	    fprintf (stderr,
		"  switch with no 'case's or 'default' nor\t allowed\n");
	    break;
	case ErrParseMultTargetEleGen :
	    fprintf (stderr,
		"  multiple targets not allowed when generating elements from array\n");
	    break;
	case ErrParseEmptyTargetEleGen :
	    fprintf (stderr,
		"  empty target not allowed when generating elements from array\n");
	    break;
	case ErrParseUntypedScalarTarget :
	    fprintf (stderr,
		"  untyped scalar generator target not allowed\n");
	    break;
	case ErrParseComplexType :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  type '%s' cannot be used in complex type\n", node_str (v));
	    break;
	case ErrParseBadReduceInAccum :
	    fprintf (stderr,
		"  illegal reduction specified in accumulation\n");
	    break;
	case ErrParsePragProto :
	    fprintf (stderr,
		"  pragma not allowed on a func prototype\n");
	    break;
	case ErrScopeUndefinedFunc :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	        "  function '%s' has not been defined\n", find_sym (v));
	    break;
	case ErrScopeInconsistentDecl :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	        "  inconsistent declrations of function '%s'\n", find_sym (v));
	    break;
	case ErrScopeNeedTargetType :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	        "  need a type for target variable '%s'\n", find_sym (v));
	    break;
	case ErrScopeFuncNameMismatch :
	    fprintf (stderr,
	        "  mismatch between names in func return type and func def\n");
	    break;
	case ErrScopeUndeclVar :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	        "  variable '%s' not in scope\n", find_sym (v));
	    break;
	case ErrScopeIllegalNextified :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	        "  variable '%s' must be nextified if used as 'final'\n", find_sym (v));
	    break;
	case ErrScopeDupFuncName :
	    fprintf (stderr,
	        "  duplicate function name\n");
	    break;
	case ErrTypePass1ElegenStepMV :
	    fprintf (stderr,
		"  element generator size expression cannot be a multiple value\n");
    	    break;
	case ErrTypePass1SlicegenStepMV :
	    fprintf (stderr,
		"  slice generator size expression cannot be a multiple value\n");
    	    break;
	case ErrTypePass1WindowSizeMV :
	    fprintf (stderr,
		"  window size expression cannot be a multiple value\n");
    	    break;
	case ErrTypePass1CallNonFunc :
	    fprintf (stderr,
		"  function call must reference a function name\n");
    	    break;
	case ErrTypePass1FuncArity :
	    fprintf (stderr,
		"  function arity mismatch between return values and declaration\n");
    	    break;
	case ErrTypePass1PrintAssertPredMV :
	    fprintf (stderr,
		"  print or assert statement has a multiple-valued predicate\n");
    	    break;
	case ErrTypePass1PrintAssertValMV :
	    fprintf (stderr,
		"  print or assert statement has a multiple-valued item\n");
    	    break;
	case ErrTypePass1UnaryValMV :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  operator '%s' is given a multiple value\n", op_str (v));
    	    break;
	case ErrTypePass1BinaryValMV :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  operator '%s' is given a multiple value\n", op_str (v));
    	    break;
	case ErrTypePass1TrinaryValMV :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  operator '%s' is given a multiple value\n", op_str (v));
    	    break;
	case ErrTypePass1CastMV :
	    fprintf (stderr,
		"  a typecast is given a multiple value\n");
    	    break;
	case ErrTypePass1ReductionArg0MV :
	    fprintf (stderr,
		"  a reduction's first arg is a multiple value\n");
    	    break;
	case ErrTypePass1ReductionArg1MV :
	    fprintf (stderr,
		"  a reduction's second arg is a multiple value\n");
    	    break;
	case ErrTypePass1ReductionArg2MV :
	    fprintf (stderr,
		"  a reduction's third arr is a multiple value\n");
    	    break;
	case ErrTypePass1ValsAt1MV :
	    fprintf (stderr,
		"  a 'vals-at' capture expression is a multiple value\n");
    	    break;
	case ErrTypePass1ArrayRefMV :
	    fprintf (stderr,
		"  an array reference taken on a multiple value\n");
    	    break;
	case ErrTypePass1ArraySubscrMV :
	    fprintf (stderr,
		"  an array subscript expr is a multiple value\n");
    	    break;
	case ErrTypePass1CondPredMV :
	    fprintf (stderr,
		"  conditional's predicate is a multiple value\n");
    	    break;
	case ErrTypePass1WhilePredMV :
	    fprintf (stderr,
		"  while loop's predicate is a multiple value\n");
    	    break;
	case ErrTypePass1SwitchExprMV :
	    fprintf (stderr,
		"  switch's expr is a multiple value\n");
    	    break;
	case ErrTypePass1ParamMV :
	    fprintf (stderr,
		"  function parameter is a multiple value\n");
    	    break;
	case ErrTypePass1WindowSourceMV :
	    fprintf (stderr,
		"  window generator's source array is a multiple value\n");
    	    break;
	case ErrTypePass1WindowStepMV :
	    fprintf (stderr,
		"  window generator's step expr is a multiple value\n");
    	    break;
	case ErrTypePass1ScalarGenExprMV :
	    fprintf (stderr,
		"  scalar generator's source expr is a multiple value\n");
    	    break;
	case ErrTypePass1ScalarGenStepMV :
	    fprintf (stderr,
		"  scalar generator's step expr is a multiple value\n");
    	    break;
	case ErrTypePass1EleGenSourceMV :
	    fprintf (stderr,
		"  element generator's source is a multiple value\n");
    	    break;
	case ErrTypePass1SliceGenSourceMV :
	    fprintf (stderr,
		"  slice generator's source is a multiple value\n");
    	    break;
	case ErrTypePass1ArrayDefValMV :
	    fprintf (stderr,
		"  array constant's value is a multiple value\n");
    	    break;
	case ErrTypePass1AssignMismatch :
	    fprintf (stderr,
		"  count mismatch across assignment\n");
    	    break;
	case ErrTypePass1ExtentsNonarray :
	    fprintf (stderr,
		"  taking extents of non-array\n");
    	    break;
	case ErrTypePass1UnbalancedCond :
	    fprintf (stderr,
		"  count mismatch on true and false parts of conditional\n");
    	    break;
	case ErrTypePass1SwitchValMismatch :
	    fprintf (stderr,
		"  count mismatch among various 'case's of switch\n");
    	    break;
	case ErrTypePass1SwitchDefaultMismatch :
	    fprintf (stderr,
		"  count mismatch between cases and default of switch\n");
    	    break;
	case ErrTypePass1ScalarGenMismatch :
	    fprintf (stderr,
		"  count mismatch between scalar generator's sources and targets\n");
    	    break;
	case ErrTypePass1ScalarGenStepMismatch :
	    fprintf (stderr,
		"  count mismatch between scalar gen's targets and steps\n");
    	    break;
	case ErrTypePass1ParamCount :
	    fprintf (stderr,
		"  function call has wrong number of arguments\n");
    	    break;
	case ErrTypePass2ConcatMaskNeeds1dArray :
	    fprintf (stderr,
		"  mask on 'concat' must be 1D array\n");
    	    break;
	case ErrTypePass2StackSize :
	    fprintf (stderr,
		"  internal compiler error: 'G_STK_SIZE' must be increased\n");
    	    break;
	case ErrTypePass2ConcatGenRank :
	    fprintf (stderr,
		"  concat needs loop rank of one; use 'tile' instead\n");
    	    break;
	case ErrTypePass2CastArrayToScalar :
	    fprintf (stderr,
		"  cannot cast an array to a scalar type\n");
    	    break;
	case ErrTypePass2CastArrayToArrayRankMismatch :
	    fprintf (stderr,
		"  array-to-array cast must have same rank\n");
    	    break;
	case ErrTypePass2CastScalarToArray :
	    fprintf (stderr,
		"  cannot cast a scalar to an array\n");
    	    break;
	case ErrTypePass2LoopIndScalar :
	    fprintf (stderr,
		"  'loop_indices' target variable must be scalar\n");
    	    break;
	case ErrTypePass2LoopIndDimMismatch :
	    fprintf (stderr,
		"  'loop_indices' target variable count mismatch\n");
    	    break;
	case ErrTypePass2LoopIndOutsideOfLoop :
	    fprintf (stderr,
		"  'loop_indices' occurs outside of a 'for' loop\n");
    	    break;
	case ErrTypePass2ElegenStepExprNeeded :
	    fprintf (stderr,
		"  'step' expression cannot be empty\n");
    	    break;
	case ErrTypePass2ElegenRankStepMismatch :
	    fprintf (stderr,
		"  'step' spec rank does not match array\n");
    	    break;
	case ErrTypePass2ElegenRankAtMismatch :
	    fprintf (stderr,
		"  element generator source rank and 'at' spec don't match\n");
    	    break;
	case ErrTypePass2ElegenSourceArray :
	    fprintf (stderr,
		"  element generator source must be an array\n");
    	    break;
	case ErrTypePass2ElegenExtract :
	    fprintf (stderr,
		"  extract pattern present for element generator\n");
    	    break;
	case ErrTypePass2SlicegenStepScalar :
	    fprintf (stderr,
		"  'step' expression must be a scalar value\n");
    	    break;
	case ErrTypePass2SlicegenStepExprNotAllowed :
	    fprintf (stderr,
		"  'step' expression in a ':' dimension not allowed\n");
    	    break;
	case ErrTypePass2SlicegenStepExprNeeded :
	    fprintf (stderr,
		"  'step' expression needed in a '~' dimension\n");
    	    break;
	case ErrTypePass2SlicegenRankStepMismatch :
	    fprintf (stderr,
		"  'step' spec rank does not match array\n");
    	    break;
	case ErrTypePass2SlicegenNoTilde :
	    fprintf (stderr,
		"  extract pattern must contain at least one '~'\n");
    	    break;
	case ErrTypePass2SlicegenNoExtract :
	    fprintf (stderr,
		"  extract pattern absent for slice genrator\n");
    	    break;
	case ErrTypePass2SlicegenRankMismatch :
	    fprintf (stderr,
		"  slice generator extraction pattern rank mismatch\n");
    	    break;
	case ErrTypePass2SlicegenRankAtMismatch :
	    fprintf (stderr,
		"  slice generator source rank and 'at' spec don't match\n");
    	    break;
	case ErrTypePass2SlicegenAtEmpty :
	    fprintf (stderr,
		"  slice generator 'at' target in a ':' dimension\n");
    	    break;
	case ErrTypePass2SlicegenSourceArray :
	    fprintf (stderr,
		"  slice generator source must be an array\n");
    	    break;
	case ErrTypePass2WindowSizeScalar :
	    fprintf (stderr,
		"  window size expression must be scalar\n");
    	    break;
	case ErrTypePass2ConcatRankMismatch :
	    fprintf (stderr,
		"  rank mismatch between args of array_concat\n");
    	    break;
	case ErrTypePass2FuncRetMismatch :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  return value of function, position %d, does not match decl\n", v);
    	    break;
	case ErrTypePass2AssignValMismatch :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  inconsistency across assignment, position %d\n", v);
    	    break;
	case ErrTypePass2PrintAssertPredScalar :
	    fprintf (stderr,
		"  predicate of print or assert must be scalar\n");
    	    break;
	case ErrTypePass2AccumSourceLabelMismatch :
	    fprintf (stderr,
		"  accum source array and label array must have same rank\n");
    	    break;
	case ErrTypePass2ReductionSourceMaskMismatch :
	    fprintf (stderr,
		"  array reduction source and mask arrays must have same rank\n");
    	    break;
	case ErrTypePass2ValsAtScalar :
	    fprintf (stderr,
		"  captured value for 'vals-at' must be scalar\n");
    	    break;
	case ErrTypePass2SubscriptRankMismatch :
	    fprintf (stderr,
		"  array subscripts don't match array's rank\n");
    	    break;
	case ErrTypePass2CondTrueFalseInconsistent :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  inconsistent value across two sides of conditional, position %d\n", v);
    	    break;
	case ErrTypePass2SwitchValInconsistent :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  switch value inconsistency, position %d\n", v);
    	    break;
	case ErrTypePass2CallArgIncompatible :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  arg %d is incompatible with func's declaration\n", v);
    	    break;
	case ErrTypePass2DotProdRanks :
	    fprintf (stderr,
		"  inconsistent ranks in dot products\n");
    	    break;
	case ErrTypePass2WindowSourceTarget :
	    fprintf (stderr,
		"  window source and target arrays must have same rank\n");
    	    break;
	case ErrTypePass2WindowSourceStep :
	    fprintf (stderr,
		"  window source and step spec must have same rank\n");
    	    break;
	case ErrTypePass2WindowSourceAt :
	    fprintf (stderr,
		"  window source and 'at' spec must have same rank\n");
    	    break;
	case ErrTypePass2VectorRank :
	    fprintf (stderr,
		"  vector specified, but rank not equal to one\n");
    	    break;
	case ErrTypePass2MatrixRank :
	    fprintf (stderr,
		"  matrix specified, but rank not equal to two\n");
    	    break;
	case ErrTypePass2CubeRank :
	    fprintf (stderr,
		"  cube specified, but rank not equal to three\n");
    	    break;
	case ErrTypePass2ArrConstDimMismatch :
	    fprintf (stderr,
		"  array constant dimension mismatch\n");
    	    break;
	case ErrTypePass2UnaryOpScalar :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  unary operator '%s' needs scalar value\n", op_str (v));
    	    break;
	case ErrTypePass2ConcatNeedsArray :
	    fprintf (stderr,
		"  concat operator works only on array components\n");
    	    break;
	case ErrTypePass2OpNeedsScalars :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  operator '%s' needs scalar operands\n", op_str (v));
    	    break;
	case ErrTypePass2PerimSourceArray :
	    fprintf (stderr,
		"  'array_conperim' needs array as first arg");
    	    break;
	case ErrTypePass2PerimValScalar :
	    fprintf (stderr,
		"  'array_conperim' needs scalar value as second arg");
    	    break;
	case ErrTypePass2PerimWidthScalar :
	    fprintf (stderr,
		"  'array_conperim' needs scalar perimeter width as third arg");
    	    break;
	case ErrTypePass2AccumRangeScalar :
	    fprintf (stderr,
		"  accum range value must be scalar\n");
    	    break;
	case ErrTypePass2AccumLabelArray :
	    fprintf (stderr,
		"  accum label must be an array\n");
    	    break;
	case ErrTypePass2LoopAccumValScalar :
	    fprintf (stderr,
		"  accum range value must be scalar\n");
    	    break;
	case ErrTypePass2LoopAccumLabelScalar :
	    fprintf (stderr,
		"  accum label must be a scalar\n");
    	    break;
	case ErrTypePass2CastOnScalar :
	    fprintf (stderr,
		"  typecast allowed only on scalar values\n");
    	    break;
	case ErrTypePass2LoopReductionScalar :
	    fprintf (stderr,
		"  loop reduction may be applied only to scalar\n");
    	    break;
	case ErrTypePass2LoopReductionMaskScalar :
	    fprintf (stderr,
		"  loop reduction mask must be scalar\n");
    	    break;
	case ErrTypePass2LoopHistRangeScalar :
	    fprintf (stderr,
		"  loop histogram range must be scalar\n");
    	    break;
	case ErrTypePass2ArrayReductionArray :
	    fprintf (stderr,
		"  source of array reduction must be an array\n");
    	    break;
	case ErrTypePass2ArrayReductionMaskArray :
	    fprintf (stderr,
		"  array reduction mask must be an array\n");
    	    break;
	case ErrTypePass2ArrayHistRangeScalar :
	    fprintf (stderr,
		"  array histogram range must be a scalar\n");
    	    break;
	case ErrTypePass2TripleScalar :
	    fprintf (stderr,
		"  array expression in triple must be a scalar\n");
    	    break;
	case ErrTypePass2ArrayIndexScalar :
	    fprintf (stderr,
		"  array index must be a scalar\n");
    	    break;
	case ErrTypePass2ArrayRefSource :
	    fprintf (stderr,
		"  source of an array reference must be an array\n");
    	    break;
	case ErrTypePass2ExtentsArray :
	    fprintf (stderr,
		"  extents may be applied only to an array\n");
    	    break;
	case ErrTypePass2CondPredScalar :
	    fprintf (stderr,
		"  conditional predicate must be scalar\n");
    	    break;
	case ErrTypePass2WhilePredScalar :
	    fprintf (stderr,
		"  while loop predicate must be scalar\n");
    	    break;
	case ErrTypePass2SwitchExprScalar :
	    fprintf (stderr,
		"  switch expression must be scalar\n");
    	    break;
	case ErrTypePass2WindowSourceArray :
	    fprintf (stderr,
		"  window generator source must be an array\n");
    	    break;
	case ErrTypePass2WindowStepScalar :
	    fprintf (stderr,
		"  window generator step expression must be scalar\n");
    	    break;
	case ErrTypePass2ScalarGenSourceScalar :
	    fprintf (stderr,
		"  scalar generator source expression must be scalar\n");
    	    break;
	case ErrTypePass2ArrayEleScalar :
	    fprintf (stderr,
		"  array element may be only scalar\n");
    	    break;
	case ErrTypePass2DupCaseVal :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  duplicate CASE value %d\n", v);
    	    break;
	case ErrTypePass2IntrinArgScalar :
	    fprintf (stderr,
		"  intrinsic function argument must be scalar\n");
    	    break;
	case ErrTypePass3ConcatMaskNeedsBool :
	    fprintf (stderr,
	          "  mask for 'concat' must have 'bool' components\n");
	    break;
	case ErrTypePass3ArithIllegalType :
	    fprintf (stderr,
	          "  arithmetic operation on illegal type\n");
	    break;
	case ErrTypePass3ValsAtIncompatTypes :
	    fprintf (stderr,
	          "  collected values for 'vals-at' must be compatible\n");
	    break;
	case ErrTypePass3ElegenStepInt :
	case ErrTypePass3SlicegenStepInt :
	    fprintf (stderr,
	          "  generator step expressions must be int or uint\n");
	    break;
	case ErrTypePass3WindowSizeNeedsInt :
	    fprintf (stderr,
	          "  window size expressions must be int or uint\n");
	    break;
	case ErrTypePass3ConcatCompatible :
	    fprintf (stderr,
	          "  types of args to array_concat are incompatible\n");
	    break;
	case ErrTypePass3RetDeclMismatch :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	          "  return value, position %d, of function doesn't match its decl\n", v);
    	    break;
	case ErrTypePass3TypeAcrossAssign :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	          "  type inconsistency, position %d, across ASSIGN\n", v);
    	    break;
	case ErrTypaPass3PrintAssertPredBool :
	    fprintf (stderr,
	          "  predicate in PRINT or ASSERT must be bool\n");
    	    break;
	case ErrTypePass3NotNeedsBool :
	    fprintf (stderr,
	          "  '!' needs a Bool operand\n");
    	    break;
	case ErrTypePass3NegateIllegalType :
	    fprintf (stderr,
	          "  illegal type for 'negate' operand\n");
    	    break;
	case ErrTypePass3RealImag :
	    fprintf (stderr,
		  "  cannot take real or imag of a non-complex value\n");
    	    break;
	case ErrTypePass3ASMDCompatible :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	          "  arithmetic operator '%s' has incompatible operands\n", op_str (v));
    	    break;
	case ErrTypePass3ModOperand :
	    fprintf (stderr,
	          "  illegal operand types for mod operator\n");
    	    break;
	case ErrTypePass3IneqIncompatible :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	          "  inequality operator '%s' has incompatible operands\n", op_str (v));
    	    break;
	case ErrTypePass3IneqComplex :
	    fprintf (stderr,
	          "  can't do inequality comparison on complex numbers\n");
    	    break;
	case ErrTypePass3EqCompatible :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	          "  operator '%s' has incompatible operands\n", op_str (v));
    	    break;
	case ErrTypePass3AndOrBool :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	          "  operator '%s' must have Bool operands\n", op_str (v));
    	    break;
	case ErrTypePass3BitOperands :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	          "  operator '%s' must have type bits operands\n", op_str (v));
    	    break;
	case ErrTypePass3ShiftOperands :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	          "  shift operator '%s' must have type bits operand and Int/Uint operand\n", op_str (v));
    	    break;
	case ErrTypePass3ComplexIncompatible :
	    fprintf (stderr,
	          "  complex constructor has incompatible operands\n");
    	    break;
	case ErrTypePass3ComplexComponent :
	    fprintf (stderr,
	          "  illegal component for making a complex\n");
    	    break;
	case ErrTypePass3PerimeterWidthType :
	    fprintf (stderr,
	          "  perimeter width value must be Int/Uint\n");
    	    break;
	case ErrTypePass3PerimeterIncompatible :
	    fprintf (stderr,
	          "  incompatible types between array and perimeter values\n");
    	    break;
	case ErrTypePass3AccumRangeInteger :
	    fprintf (stderr,
	          "  range expression for ACCUM must be integer\n");
    	    break;
	case ErrTypePass3AccumLabelInteger :
	    fprintf (stderr,
	          "  label expression for ACCUM must be integer\n");
    	    break;
	case ErrTypePass3IllegalTypecast :
	    fprintf (stderr,
	          "  illegal typecast\n");
    	    break;
	case ErrTypePass3ReduceNeedsNumeric :
	    fprintf (stderr,
	          "  reduction needs a numeric type\n");
    	    break;
	case ErrTypePass3MaskNeedsBool :
	    fprintf (stderr,
	          "  mask expression must be Bool\n");
    	    break;
	case ErrTypePass3ReduceNeedsNoncomplex :
	    fprintf (stderr,
	          "  reduction needs a non-complex type\n");
    	    break;
	case ErrTypePass3ReduceNeedsBoolBits :
	    fprintf (stderr,
	          "  reduction needs bool or bits type\n");
    	    break;
	case ErrTypePass3HistNeedsInt :
	    fprintf (stderr,
	          "  histogram needs Uint/Int type\n");
    	    break;
	case ErrTypePass3HistExprNeedsInt :
	    fprintf (stderr,
	          "  histogram range expression must be Int/Uint\n");
    	    break;
	case ErrTypePass3ArrayIndexNeedsInt :
	    fprintf (stderr,
	          "  array index expr must be Int/Uint\n");
    	    break;
	case ErrTypePass3CondNeedsBool :
	    fprintf (stderr,
	          "  conditional needs boolean expression for predicate\n");
    	    break;
	case ErrTypePass3CondTypeIncompatibleTF :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	          "  incompatible types, position %d, across T/F sides of conditional\n", v);
    	    break;
	case ErrTypePass3WhileNeedsBool :
	    fprintf (stderr,
	          "  while loop needs boolean expression for predicate\n");
    	    break;
	case ErrTypePass3SwitchExprType :
	    fprintf (stderr,
	          "  illegal type in switch expression\n");
    	    break;
	case ErrTypePass3SwitchValueIncompatible :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	          "  type incompatibility in switch return, position %d\n", v);
    	    break;
	case ErrTypePass3ParamIncompatible :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
	          "  incompatible type in param %d of func call\n", v);
    	    break;
	case ErrTypePass3ElegenGenTargetIncompatible :
	case ErrTypePass3SlicegenGenTargetIncompatible :
	    fprintf (stderr,
	          "  type incompatibility between generator expression and its target\n");
    	    break;
	case ErTypePass3ElegenAtInt :
	case ErTypePass3SlicegenAtInt :
	    fprintf (stderr,
	          "  generator 'at' target must be Int/Uint\n");
    	    break;
	case ErrTypePass3ScalargenTargetInt :
	    fprintf (stderr,
	          "  scalar generator target must be integer\n");
    	    break;
	case ErrTypePass3ScalargenSourceInt :
	    fprintf (stderr,
	          "  scalar generator source expression must be integer\n");
    	    break;
	case ErrTypePass3ScalargenStepInt :
	    fprintf (stderr,
	          "  scalar generator step expression must be integer\n");
    	    break;
	case ErrTypePass3WindowgenTargetIncompatible :
	    fprintf (stderr,
	          "  type incompatibility between window generator expression and its target\n");
    	    break;
	case ErrTypePass3WindowgenStepInt :
	    fprintf (stderr,
	          "  window generator step expression must be integer\n");
    	    break;
	case ErrTypePass3WindowgenAtInt :
	    fprintf (stderr,
	          "  window generator 'at' target must be Int or Uint\n");
    	    break;
	case ErrTypePass3ArraydefIncompatible :
	    fprintf (stderr,
	          "  incompatible type in array definition\n");
    	    break;
	case ErrTypePass3IntrinArg :
	    fprintf (stderr,
	          "  intrinsic function argument must be non-complex numeric\n");
    	    break;
	case ErrFlowGenVarUsedInGen :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  generator target variable '%s' is used in its generator\n", find_sym (v));
	    break;
	case ErrFlowIllegalNextifiedVar :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  variable '%s' cannot be nextified\n", find_sym (v));
	    break;
	case ErrFlowLocalNextifiedVar :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  local variable '%s' cannot be nextified\n", find_sym (v));
	    break;
	case ErrFlowAlreadyNextifiedVar :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  variable '%s' is nextified more than once\n", find_sym (v));
	    break;
	case ErrFlowLoopVarUsedInRange :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  loop variable '%s' is used in a 'range' expression\n", find_sym (v));
	    break;
	case ErrFlowRecursion :
	    fprintf (stderr,
		"  recursion is not allowed\n");
	    break;
	case ErrDdcf2cFileOpen :
	    fprintf (stderr,
		"  couldn't open output file in 'ddcf2c'\n");
	    break;
	case ErrParseRangeOnlyForHist :
	    fprintf (stderr,
		"  range specification applies only to histograms\n");
	    break;
	case ErrParseHistNeedsRange :
	    fprintf (stderr,
		"  histogram needs range specification\n");
	    break;
	case ErrParseConcatNeedsTwoArrays :
	    fprintf (stderr,
		"  array_concat needs two arrays\n");
	    break;
	case ErrDdcf2cDivWidth :
	    fprintf (stderr,
		"  fixed point division internal width exceeds 32\n");
	    break;
	case ErrSizePropSizeMismatch :
	    fprintf (stderr,
		"  inconsistency in array size spec\n");
	    break;
	case ErrSizePropSizeMismatchVals :
	    {
	    int v0, v1, dim;
    	    dim = va_arg (ap, int);
    	    v0 = va_arg (ap, int);
    	    v1 = va_arg (ap, int);
	    fprintf (stderr,
		"  inconsistency in array size spec, dimension %d, value %d vs %d\n", dim, v0, v1);
	    }
	    break;
	case ErrSizePropConcatClash :
    	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  size constants clash in dimension %d for array_concat\n", v);
	    break;
	case ErrSizePropDotGenMismatch :
	    fprintf (stderr,
		"  inconsistent dimension size in dot product\n");
	    break;
	case ErrSizePropConPerimLessThanZero :
	    fprintf (stderr,
		"  array_conperim source dimension is less than zero\n");
	    break;
	case ErrArrayConstProp :
	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  array index %d out of bounds\n", v);
	    break;
	case ErrDdcf2dfgNoCastYet :
	    {
	    char *tstr;
	    tstr = va_arg (ap, char*);
	    fprintf (stderr,
		"  in conversion to DFG, haven't implemented cast from type %s yet\n", tstr);
	    }
	    break;
	case ErrDdcf2dfgWindowIndex :
	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  window array index %d out of bounds\n", v);
	    break;
	case ErrDdcf2dfgVectorSliceIndex :
	    v = va_arg (ap, int);
	    fprintf (stderr,
		"  vector slice index %d out of bounds\n", v);
	    break;
	case ErrBinaryReadFiletype :
	    fprintf (stderr,
		"  filetype problem while reading binary file for lookup table building\n");
	    break;
	case ErrBinaryReadFormat :
	    fprintf (stderr,
		"  file format problem while reading binary file for lookup table building\n");
	    break;
	case ErrBinaryReadNoData :
	    fprintf (stderr,
		"  unexpected end of file while reading binary file for lookup table building\n");
	    break;
	case ErrBinaryReadLineLengthOverflow :
	    fprintf (stderr,
		"  line length overflow while reading binary file for lookup table building\n");
	    break;
	case ErrBinaryReadFail :
	    fprintf (stderr,
		"  'read' failure while reading binary file for lookup table building\n");
	    break;
	case ErrConsistConstAndEdge :
	    {
	    int nd, pt;
	    char *ty;
	    nd = va_arg (ap, int);
	    ty = va_arg (ap, char*);
	    pt = va_arg (ap, int);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  node %d, type %s, input port %d has both constant and edge\n", nd, ty, pt);
	    }
	    break;
	case ErrConsistNoConstOrEdge :
	    {
	    int nd, pt;
	    char *ty;
	    nd = va_arg (ap, int);
	    ty = va_arg (ap, char*);
	    pt = va_arg (ap, int);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  node %d, type %s, input port %d has no constant or edge\n", nd, ty, pt);
	    break;
	    }
	case ErrConsistMultBackEdges :
	    {
	    int nd, pt;
	    char *ty;
	    nd = va_arg (ap, int);
	    ty = va_arg (ap, char*);
	    pt = va_arg (ap, int);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  node %d, type %s, input port %d has multiple back edges\n", nd, ty, pt);
	    }
	    break;
	case ErrConsistGraphInputWrongType :
	    {
	    int nd, pt;
	    char *ty;
	    nd = va_arg (ap, int);
	    ty = va_arg (ap, char*);
	    pt = va_arg (ap, int);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  graph node %d input port %d has non-input node type %s\n", nd, pt, ty);
	    }
	    break;
	case ErrConsisiGraphOutputWrongType :
	    {
	    int nd, pt;
	    char *ty;
	    nd = va_arg (ap, int);
	    ty = va_arg (ap, char*);
	    pt = va_arg (ap, int);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  graph node %d output port %d has non-input node type %s\n", nd, pt, ty);
	    }
	    break;
	case ErrConsistBadOwner :
	    {
	    int nd;
	    char *ty;
	    nd = va_arg (ap, int);
	    ty = va_arg (ap, char*);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  graph node %d, type %s, has ownership error\n", nd, ty);
	    }
	    break;
	case ErrConsistEdgeToVoid :
	    {
	    int nd, pt, snd;
	    char *ty;
	    ty = va_arg (ap, char*);
	    nd = va_arg (ap, int);
	    pt = va_arg (ap, int);
	    snd = va_arg (ap, int);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  back edge from %s node %d, port %d, goes to ND_VOIDED node %d\n", ty, nd, pt, snd);
	    }
	    break;
	case ErrConsistEdgeToNonexistentPort :
	    {
	    int nd, pt, snd, spt;
	    nd = va_arg (ap, int);
	    pt = va_arg (ap, int);
	    snd = va_arg (ap, int);
	    spt = va_arg (ap, int);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  back edge from <%d.%d> goes to nonexistent output <%d.%d>\n", nd, pt, snd, spt);
	    }
	    break;
	case ErrConsistNoTargetForBackedge :
	    {
	    int nd, pt, snd, spt;
	    nd = va_arg (ap, int);
	    pt = va_arg (ap, int);
	    snd = va_arg (ap, int);
	    spt = va_arg (ap, int);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  no target to match back edge <%d.%d> to <%d.%d>\n", nd, pt, snd, spt);
	    }
	    break;
	case ErrConsistTargetToVoid :
	    {
	    int nd, pt, snd;
	    nd = va_arg (ap, int);
	    pt = va_arg (ap, int);
	    snd = va_arg (ap, int);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  target from <%d.%d> goes to ND_VOIDED node %d\n", nd, pt, snd);
	    }
	    break;
	case ErrConsistTargetToNonexistentInput :
	    {
	    int nd, pt, snd, spt;
	    nd = va_arg (ap, int);
	    pt = va_arg (ap, int);
	    snd = va_arg (ap, int);
	    spt = va_arg (ap, int);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  target from <%d.%d> goes to nonexistent input <%d.%d>\n", nd, pt, snd, spt);
	    }
	    break;
	case ErrConsistNoEdgeMatchTarget :
	    {
	    int nd, pt, snd, spt;
	    nd = va_arg (ap, int);
	    pt = va_arg (ap, int);
	    snd = va_arg (ap, int);
	    spt = va_arg (ap, int);
	    fprintf (stderr, "  compiler malfunction, DDCF not consistent\n");
	    fprintf (stderr,
		"  no back edge to match target <%d.%d> to <%d.%d>\n", nd, pt, snd, spt);
	    }
	    break;
	case ErrLookupBadInType :
	    {
	    char *knd;
	    knd = va_arg (ap, char*);
	    fprintf (stderr,
		"  %s is illegal input type for a lookup function\n", knd);
	    }
	    break;
	case ErrLookupBadOutType :
	    {
	    char *knd;
	    knd = va_arg (ap, char*);
	    fprintf (stderr,
		"  %s is illegal output type for a lookup function\n", knd);
	    }
	    break;
	case ErrLookupInBitWidth :
	    {
	    int totwidth;
	    totwidth = va_arg (ap, int);
	    fprintf (stderr,
		"  lookup function total input bit width %d too big\n", totwidth);
	    }
	    break;
	case ErrLookupMVR :
	    fprintf (stderr,
		"  can't create a lookup table from a multiple-value-return function\n");
	    break;
	case ErrLookupRetNonScalar :
	    fprintf (stderr,
		"  can't create a lookup table from a function returning a non-scalar\n");
	    break;
	case ErrLookupHasFuncCall :
	    fprintf (stderr,
		"  can't create a lookup table because function contains a function call\n");
	    break;
	case ErrLookupFileOpen :
	    {
	    char *tmp_src_name;
	    tmp_src_name = va_arg (ap, char*);
	    fprintf (stderr,
		"  couldn't open file '%s' for writing\n", tmp_src_name);
	    }
	    break;
	case ErrLookupSassyhome :
	    fprintf (stderr,
		"  error in 'dump_and_compile' looking for SASSYHOME\n");
	    break;
	case ErrLookupSpawnFail :
	    fprintf (stderr,
		"  spawned compile failed in 'dump_and_compile'\n");
	    break;
	case ErrLookupRemoveFile :
	    {
	    char *tmp_src_name;
	    tmp_src_name = va_arg (ap, char*);
	    fprintf (stderr,
		"  in 'dump_and_compile', failed to remove temp file '%s'\n", tmp_src_name);
	    }
	    break;
	case ErrLookupExec :
	    {
	    char *tmp_exec_name;
	    tmp_exec_name = va_arg (ap, char*);
	    fprintf (stderr,
		"  error while executing %s\n", tmp_exec_name);
	    }
	    break;
	case ErrLookupTmpRemove :
	    {
	    char *tmp_exec_name;
	    tmp_exec_name = va_arg (ap, char*);
	    fprintf (stderr,
		"  in 'dump_and_compile', failed to remove temp file '%s'\n", tmp_exec_name);
	    }
	    break;
	case ErrLookupDataOpen :
	    {
	    char *tmp_dt_file;
	    tmp_dt_file = va_arg (ap, char*);
	    fprintf (stderr,
		"  couldn't open lookup data file `%s'\n", tmp_dt_file);
	    }
	    break;
	case ErrLookupTmpFileRemove :
	    {
	    char *tmp_dt_file;
	    tmp_dt_file = va_arg (ap, char*);
	    fprintf (stderr,
		"  in 'fetch_inputs', failed to remove temp file '%s'\n", tmp_dt_file);
	    }
	    break;
	case ErrMainOptNotStable :
	    fprintf (stderr,
		"  optimizations failed to stabilize\n");
	    break;
	case ErrConcatMaskedArrDefExtents :
	    {
	    int sz0, sz1;
	    sz0 = va_arg (ap, int);
	    sz1 = va_arg (ap, int);
	    fprintf (stderr,
		"  array size mismatch, size %d vs size %d\n", sz0, sz1);
	    }
	    break;
	}

    fflush (stderr);

    leave_error :
    if (level == Deadly)
        exit (-1);

    va_end (ap);
    }

char *op_str (Node ntype)
    {
    switch (ntype)
        {
	case NOT :
	    return "!";
	case NEG :
	    return "-";
	case REAL :
	    return "real";
	case IMAG :
	    return "imag";
	case CONSTRUCT_ARRAY :
	    return "array";
	case CONSTRUCT_VECTOR :
	    return "vector";
	case CONSTRUCT_MATRIX :
	    return "matrix";
	case CONSTRUCT_CUBE :
	    return "cube";
	case CONSTRUCT_CONCAT :
	    return "concat";
	case ADD :
	    return "+";
        case SUB :
	    return "-";
        case MUL :
	    return "*";
        case DIV :
	    return "/";
	case MOD :
	    return "%";
        case LT :
	    return "<";
        case GT :
	    return ">";
        case LE :
	    return "<=";
        case GE :
	    return ">=";
        case NEQ :
	    return "!=";
        case EQ :
	    return "==";
        case AND :
	    return "&&";
	case OR :
	    return "||";
        case BIT_AND :
	    return "&";
        case BIT_OR :
	    return "|";
        case BIT_EOR :
	    return "^";
        case LEFT_SHIFT :
	    return "<<";
        case RIGHT_SHIFT :
	    return ">>";
        case COMPLEX :
	    return "complex";
        case ARR_CONPERIM :
	    return "array_conperim";
        case ARR_ACCUM :
	    return "array_accum";
        case ACCUM :
	    return "accum";
	default :
	    return "";
	}
    }
