%module fapolicyanalyzer
%{
#include "libfapolicyd-analyzer/error_codes.h"
#include "libfapolicyd-analyzer/field_array.h"
#include "libfapolicyd-analyzer/match.h"
#include "libfapolicyd-analyzer/parse.h"
#include "libfapolicyd-analyzer/rpm.h"
#include "libfapolicyd-analyzer/sha256.h"
#include "libfapolicyd-analyzer/table.h"
%}

%include "libfapolicyd-analyzer/error_codes.h"
%include "libfapolicyd-analyzer/field_array.h"
%include "libfapolicyd-analyzer/match.h"
%include "libfapolicyd-analyzer/parse.h"
%include "libfapolicyd-analyzer/rpm.h"
%include "libfapolicyd-analyzer/sha256.h"
%include "libfapolicyd-analyzer/table.h"
