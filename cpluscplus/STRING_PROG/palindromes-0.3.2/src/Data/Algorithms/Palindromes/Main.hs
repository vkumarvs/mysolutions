-----------------------------------------------------------------------------
-- |
-- Module      :  Data.Algorithms.Palindromes.Main
-- Copyright   :  (c) 2007 - 2010 Johan Jeuring
-- License     :  BSD3
--
-- Maintainer  :  johan@jeuring.net
-- Stability   :  experimental
-- Portability :  portable
--
-----------------------------------------------------------------------------
module Main where

import System.Environment (getArgs)
import System.Console.GetOpt 
import System.IO

import qualified Data.ByteString as B

import Data.Algorithms.Palindromes.Options

-----------------------------------------------------------------------------
-- main
-----------------------------------------------------------------------------

handleFilesWith :: (B.ByteString -> String) -> [String] -> IO ()
handleFilesWith f = 
  let hFW filenames = 
        case filenames of
          []        ->  putStr (f B.empty)
          (fn:fns)  ->  do -- input <- B.readFile fn
                           fn' <- openFile fn ReadMode
                           hSetEncoding fn' latin1 
                           input <- B.hGetContents fn' 
                           putStrLn (f input)
                           hFW fns
  in hFW                                 

handleStandardInputWith :: (B.ByteString -> String) -> IO ()
handleStandardInputWith function = 
  do input <- B.getContents
     putStrLn (function input) 

main :: IO ()
main = do args <- getArgs
          let (optionArgs,files,errors) = getOpt Permute options args
          if not (null errors) 
            then putStrLn (concat errors) 
            else let (function,fromfile) = handleOptions optionArgs
                 in  if fromfile 
                     then handleFilesWith function files 
                     else handleStandardInputWith function 

