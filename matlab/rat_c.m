function [N,D] = rat_c(X,tol)
%RAT    Rational approximation.
%   [N,D] = RAT(X,tol) returns two integer matrices so that 
%   N./D is close to X in the sense that abs(N./D - X) <= tol.
%   The rational approximations are generated by truncating continued
%   fraction expansions.   tol = 1.e-6*norm(X(:),1) is the default.
%
%   S = RAT(X) or RAT(X,tol) returns the continued fraction 
%   representation as a string.
%
%   The same algorithm, with the default tol, is used internally
%   by MATLAB for FORMAT RAT.
%
%   Class support for input X:
%      float: double, single
%
%   See also FORMAT, RATS.

%   Copyright 1984-2011 The MathWorks, Inc. 

% Approximate x by
%
%                              1
%         d1 + ----------------------------------
%                                 1
%              d2 + -----------------------------
%                                   1
%                   d3 + ------------------------
%                                      1
%                        d4 + -------------------
%                                        1
%                             d5 + --------------
%                                           1
%                                  d6 + ---------
%                                             1
%                                       d7 + ----
%                                             d8
%
% The d's are obtained by repeatedly picking off the integer part and 
% then taking the reciprocal of the fractional part.  The accuracy of
% the approximation increases exponentially with the number of terms
% and is worst when x = sqrt(2).  For x = sqrt(2), the error with k
% terms is about 2.68*(.173)^k, which is
%
%         1    4.6364e-01
%         2    8.0210e-02
%         3    1.3876e-02
%         4    2.4006e-03
%         5    4.1530e-04
%         6    7.1847e-05
%         7    1.2430e-05
%         8    2.1503e-06
%         9    3.7201e-07
%        10    6.4357e-08

if nargin < 2
    tol = 1.e-6*norm(X(isfinite(X)),1);
end
if nargout > 1
    N = zeros(size(X), class(X));
    D = zeros(size(X), class(X));
else
    N = zeros(0,0,class(X)); 
end
for j = 1:numel(X)
    x = X(j);
    if ~isfinite(x), % Special case for inf, -inf, NaN
        error('ERROR: NOT SUPPORTED - INFINITE');
    else
        k = 0;
        C = [1 0; 0 1];    % [n(k) n(k-1); d(k) d(k-1)];
        while 1
            k = k + 1;
            neg = x < 0;
            d = round(x);
            if ~isinf(x),
                x = x - d;
                C = [C*[d;1] C(:,1)];
            else % Special case for +/- inf
                C = [[x;0] C(:,1)];
            end
            if nargout <= 1
                error('ERROR: NOT SUPPORTED - OUT ARGS UNMATCHED');
            end
            if (x==0) || (abs(C(1,1)/C(2,1) - X(j)) <= max(tol,eps(X(j))))
               break
            end
            x = 1/x;
        end
        if nargout > 1
            N(j) = C(1,1)/sign(C(2,1));
            D(j) = abs(C(2,1));
        else
            k = length(s)-size(N,2);
            N = char([N ' '*ones(j-1,k); s ' '*ones(1,-k)]);
        end
    end
end