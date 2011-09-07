typedef enum {
    Fatal,
    Deadly
    } ErrLevel;

typedef enum {
    ErrMallocFailure,
    ErrMainOpenSource,
    ErrMainBufSizeError,
    ErrMainOpenDdcfDump,
    ErrMainMainDashM,
    ErrMainMissingDashM,
    ErrScanDanglingString,
    ErrScanDanglingComment,
    ErrScanInvalidChar,
    ErrScanBitWidth,
    ErrScanBitWidthFrac,
    ErrScanBitWidthLow,
    ErrParse,
    ErrParseMaskOnStructOp,
    ErrParseArrayDefNeedsSize,
    ErrParseNoCaseOrDefault,
    ErrParseMultTargetEleGen,
    ErrParseEmptyTargetEleGen,
    ErrParseBadReduceInAccum,
    ErrParseUntypedScalarTarget,
    ErrParseComplexType,
    ErrParseRangeOnlyForHist,
    ErrParseHistNeedsRange,
    ErrParseConcatNeedsTwoArrays,
    ErrParsePragProto,
    ErrScopeUndefinedFunc,
    ErrScopeInconsistentDecl,
    ErrScopeNeedTargetType,
    ErrScopeFuncNameMismatch,
    ErrScopeUndeclVar,
    ErrScopeDupFuncName,
    ErrScopeIllegalNextified,
    ErrTypePass1ElegenStepMV,
    ErrTypePass1SlicegenStepMV,
    ErrTypePass1WindowSizeMV,
    ErrTypePass1CallNonFunc,
    ErrTypePass1FuncArity,
    ErrTypePass1PrintAssertPredMV,
    ErrTypePass1PrintAssertValMV,
    ErrTypePass1UnaryValMV,
    ErrTypePass1BinaryValMV,
    ErrTypePass1TrinaryValMV,
    ErrTypePass1CastMV,
    ErrTypePass1ReductionArg0MV,
    ErrTypePass1ReductionArg1MV,
    ErrTypePass1ReductionArg2MV,
    ErrTypePass1ValsAt1MV,
    ErrTypePass1ArrayRefMV,
    ErrTypePass1ArraySubscrMV,
    ErrTypePass1CondPredMV,
    ErrTypePass1WhilePredMV,
    ErrTypePass1SwitchExprMV,
    ErrTypePass1ParamMV,
    ErrTypePass1WindowSourceMV,
    ErrTypePass1WindowStepMV,
    ErrTypePass1ScalarGenExprMV,
    ErrTypePass1ScalarGenStepMV,
    ErrTypePass1EleGenSourceMV,
    ErrTypePass1SliceGenSourceMV,
    ErrTypePass1ArrayDefValMV,
    ErrTypePass1AssignMismatch,
    ErrTypePass1ExtentsNonarray,
    ErrTypePass1UnbalancedCond,
    ErrTypePass1SwitchValMismatch,
    ErrTypePass1SwitchDefaultMismatch,
    ErrTypePass1ScalarGenMismatch,
    ErrTypePass1ScalarGenStepMismatch,
    ErrTypePass1ParamCount,
    ErrTypePass2ConcatMaskNeeds1dArray,
    ErrTypePass2ConcatGenRank,
    ErrTypePass2CastArrayToScalar,
    ErrTypePass2CastArrayToArrayRankMismatch,
    ErrTypePass2CastScalarToArray,
    ErrTypePass2LoopIndScalar,
    ErrTypePass2LoopIndDimMismatch,
    ErrTypePass2LoopIndOutsideOfLoop,
    ErrTypePass2ElegenStepExprNeeded,
    ErrTypePass2ElegenRankStepMismatch,
    ErrTypePass2ElegenRankAtMismatch,
    ErrTypePass2ElegenSourceArray,
    ErrTypePass2ElegenExtract,
    ErrTypePass2SlicegenNoExtract,
    ErrTypePass2SlicegenStepScalar,
    ErrTypePass2SlicegenStepExprNeeded,
    ErrTypePass2SlicegenStepExprNotAllowed,
    ErrTypePass2SlicegenRankStepMismatch,
    ErrTypePass2SlicegenNoTilde,
    ErrTypePass2SlicegenRankMismatch,
    ErrTypePass2SlicegenRankAtMismatch,
    ErrTypePass2SlicegenAtEmpty,
    ErrTypePass2SlicegenSourceArray,
    ErrTypePass2WindowSizeScalar,
    ErrTypePass2ConcatRankMismatch,
    ErrTypePass2FuncRetMismatch,
    ErrTypePass2AssignValMismatch,
    ErrTypePass2PrintAssertPredScalar,
    ErrTypePass2AccumSourceLabelMismatch,
    ErrTypePass2ReductionSourceMaskMismatch,
    ErrTypePass2ValsAtScalar,
    ErrTypePass2SubscriptRankMismatch,
    ErrTypePass2CondTrueFalseInconsistent,
    ErrTypePass2SwitchValInconsistent,
    ErrTypePass2CallArgIncompatible,
    ErrTypePass2DotProdRanks,
    ErrTypePass2WindowSourceTarget,
    ErrTypePass2WindowSourceStep,
    ErrTypePass2WindowSourceAt,
    ErrTypePass2VectorRank,
    ErrTypePass2MatrixRank,
    ErrTypePass2CubeRank,
    ErrTypePass2ArrConstDimMismatch,
    ErrTypePass2UnaryOpScalar,
    ErrTypePass2ConcatNeedsArray,
    ErrTypePass2OpNeedsScalars,
    ErrTypePass2PerimSourceArray,
    ErrTypePass2PerimValScalar,
    ErrTypePass2PerimWidthScalar,
    ErrTypePass2AccumRangeScalar,
    ErrTypePass2AccumLabelArray,
    ErrTypePass2LoopAccumValScalar,
    ErrTypePass2LoopAccumLabelScalar,
    ErrTypePass2CastOnScalar,
    ErrTypePass2LoopReductionScalar,
    ErrTypePass2LoopReductionMaskScalar,
    ErrTypePass2LoopHistRangeScalar,
    ErrTypePass2ArrayReductionArray,
    ErrTypePass2ArrayReductionMaskArray,
    ErrTypePass2ArrayHistRangeScalar,
    ErrTypePass2TripleScalar,
    ErrTypePass2ArrayIndexScalar,
    ErrTypePass2ArrayRefSource,
    ErrTypePass2ExtentsArray,
    ErrTypePass2CondPredScalar,
    ErrTypePass2WhilePredScalar,
    ErrTypePass2SwitchExprScalar,
    ErrTypePass2WindowSourceArray,
    ErrTypePass2WindowStepScalar,
    ErrTypePass2ScalarGenSourceScalar,
    ErrTypePass2ArrayEleScalar,
    ErrTypePass2DupCaseVal,
    ErrTypePass2IntrinArgScalar,
    ErrTypePass3ConcatMaskNeedsBool,
    ErrTypePass3ArithIllegalType,
    ErrTypePass3ValsAtIncompatTypes,
    ErrTypePass3ElegenStepInt,
    ErrTypePass3SlicegenStepInt,
    ErrTypePass3WindowSizeNeedsInt,
    ErrTypePass3ConcatCompatible,
    ErrTypePass3RetDeclMismatch,
    ErrTypePass3TypeAcrossAssign,
    ErrTypaPass3PrintAssertPredBool,
    ErrTypePass3NotNeedsBool,
    ErrTypePass3NegateIllegalType,
    ErrTypePass3RealImag,
    ErrTypePass3ASMDCompatible,
    ErrTypePass3ModOperand,
    ErrTypePass3IneqIncompatible,
    ErrTypePass3IneqComplex,
    ErrTypePass3EqCompatible,
    ErrTypePass3AndOrBool,
    ErrTypePass3BitOperands,
    ErrTypePass3ShiftOperands,
    ErrTypePass3ComplexIncompatible,
    ErrTypePass3ComplexComponent,
    ErrTypePass3PerimeterWidthType,
    ErrTypePass3PerimeterIncompatible,
    ErrTypePass3AccumRangeInteger,
    ErrTypePass3AccumLabelInteger,
    ErrTypePass3IllegalTypecast,
    ErrTypePass3ReduceNeedsNumeric,
    ErrTypePass3MaskNeedsBool,
    ErrTypePass3ReduceNeedsNoncomplex,
    ErrTypePass3ReduceNeedsBoolBits,
    ErrTypePass3HistNeedsInt,
    ErrTypePass3HistExprNeedsInt,
    ErrTypePass3ArrayIndexNeedsInt,
    ErrTypePass3CondNeedsBool,
    ErrTypePass3CondTypeIncompatibleTF,
    ErrTypePass3WhileNeedsBool,
    ErrTypePass3SwitchExprType,
    ErrTypePass3SwitchValueIncompatible,
    ErrTypePass3ParamIncompatible,
    ErrTypePass3ElegenGenTargetIncompatible,
    ErrTypePass3SlicegenGenTargetIncompatible,
    ErTypePass3ElegenAtInt,
    ErTypePass3SlicegenAtInt,
    ErrTypePass3ScalargenTargetInt,
    ErrTypePass3ScalargenSourceInt,
    ErrTypePass3ScalargenStepInt,
    ErrTypePass3WindowgenTargetIncompatible,
    ErrTypePass3WindowgenStepInt,
    ErrTypePass3WindowgenAtInt,
    ErrTypePass3ArraydefIncompatible,
    ErrTypePass3IntrinArg,
    ErrFlowIllegalNextifiedVar,
    ErrFlowLocalNextifiedVar,
    ErrFlowAlreadyNextifiedVar,
    ErrFlowGenVarUsedInGen,
    ErrFlowLoopVarUsedInRange,
    ErrFlowRecursion,
    ErrDdcf2cFileOpen,
    ErrDdcf2cDivWidth,
    ErrSizePropSizeMismatch,
    ErrSizePropSizeMismatchVals,
    ErrSizePropConcatClash,
    ErrSizePropDotGenMismatch,
    ErrArrayConstProp,
    ErrBinaryReadFiletype,
    ErrBinaryReadFormat,
    ErrBinaryReadNoData,
    ErrBinaryReadLineLengthOverflow,
    ErrBinaryReadFail,
    ErrDdcf2dfgNoCastYet,
    ErrDdcf2dfgWindowIndex,
    ErrDdcf2dfgVectorSliceIndex,
    ErrSizePropConPerimLessThanZero,
    ErrConsistConstAndEdge,
    ErrConsistNoConstOrEdge,
    ErrConsistMultBackEdges,
    ErrConsistGraphInputWrongType,
    ErrConsisiGraphOutputWrongType,
    ErrConsistBadOwner,
    ErrConsistEdgeToVoid,
    ErrConsistEdgeToNonexistentPort,
    ErrConsistNoTargetForBackedge,
    ErrConsistTargetToVoid,
    ErrConsistTargetToNonexistentInput,
    ErrConsistNoEdgeMatchTarget,
    ErrLookupBadInType,
    ErrLookupBadOutType,
    ErrLookupInBitWidth,
    ErrLookupMVR,
    ErrLookupRetNonScalar,
    ErrLookupHasFuncCall,
    ErrLookupFileOpen,
    ErrLookupSassyhome,
    ErrLookupSpawnFail,
    ErrLookupRemoveFile,
    ErrLookupExec,
    ErrLookupTmpRemove,
    ErrLookupDataOpen,
    ErrLookupTmpFileRemove,
    ErrMainOptNotStable,
    ErrTypePass2StackSize,
    ErrConcatMaskedArrDefExtents
    } Error;

#define ExitOkay 0
#define ExitParse 1
#define ExitScope 2
#define ExitTypePass1 3
#define ExitTypes 4
#define ExitFlow 5

void handle_error (Error err, ErrLevel level, int line, char *file, char *func, ...);

extern int global_error;
extern int quiet;
