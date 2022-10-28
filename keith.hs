module Keith where

step :: [Integer] -> [Integer]
step = tail <> pure . sum

split :: Integer -> [Integer]
split = map (read . pure) . show

iterateToFixpoint :: Eq a => (a -> a) -> a -> [a]
iterateToFixpoint f a
  | f a == a = [a]
  | otherwise = a : iterateToFixpoint f (f a)

keithSequence :: Integer -> [Integer]
keithSequence n = takeWhile (<= n) $ map head $ iterateToFixpoint step $ split n

isKeith :: Integer -> Bool
isKeith n = n == last (keithSequence n)
