module Main where

import Data.List
import Math.NumberTheory.Primes.Testing
import qualified Data.Set as S

nubBySet :: Ord a => [a] -> [a]
nubBySet = S.toList . S.fromList

candidates, primeval :: Integer -> [Integer]
candidates
  = nubBySet
  . map read
  . filter (not . null)
  . concatMap tails
  . permutations
  . show
primeval = filter isPrime . candidates

primevalCount :: Integer -> Integer
primevalCount = fromIntegral . length . primeval

monotonic :: Integer -> [Integer] -> [Integer]
monotonic bestSoFar [] = []
monotonic bestSoFar (n:ns)
  | primevalCount n <= bestSoFar = monotonic bestSoFar ns
  | otherwise = n : monotonic (primevalCount n) ns

main :: IO ()
main = mapM_ print (map (\x -> (x, primevalCount x)) [1..])

  {-
insertC :: Char -> String -> [String]
insertC c [] = [[c]]
insertC c (head:tail) =
  let subinsertions = map (head :) (insertC c tail)
      newinsertion = c : head : tail
  in
  if c == head
     then subinsertions
     else newinsertion : subinsertions

insertC' :: Char -> String -> [String]
insertC' '0' (x:xs) = map (x :) (insertC' '0' xs)
insertC' c str = insertC c str

prunedPermutations :: String -> [String]
prunedPermutations [] = error "Base case!"
prunedPermutations [c] = [[c],[]]
prunedPermutations (head:tail) =
  let substrings = prunedPermutations tail
      alternatives = concatMap (insertC' head) substrings
  in
  substrings ++ alternatives
  -}
