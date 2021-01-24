import qualified Data.ByteString.Lazy as B
import qualified Data.ByteString.Builder as B
import Data.Foldable
import Data.List
import System.Process
import Text.Printf

type Pulse      = Float
type Seconds    = Float
type SampleRate = Float
type Hertz      = Float
type Semitones  = Float
type TET        = Float
type Volume     = Float
type Sound      = [Pulse]
type Chord      = Sound

outputFilePath :: FilePath
outputFilePath = "output.bin"

volume :: Volume
volume = 0.1

sampleRate :: SampleRate
sampleRate = 48000.0

pitchStandard :: Hertz
pitchStandard = 440.0

temperament :: TET
temperament = 12.0

noteFreq :: Semitones -> Hertz
noteFreq n = pitchStandard * (2 ** (1.0 / temperament)) ** n

freqSound :: Seconds -> Float -> Float -> Hertz -> Sound
freqSound duration attack release freq = map (* volume) $ zipWith3 (\x y z -> x * y * z)
                                                          attackMultipliers releaseMultipliers pureFreqSound
    where
        step = (freq * 2 * pi) / sampleRate
        pureFreqSound :: Sound
        pureFreqSound = map sin $ map (* step) [0.0 .. sampleRate * duration]
        attackMultipliers :: [Volume]
        attackMultipliers = map (min 1.0) [0.0, attack ..]
        releaseMultipliers :: [Volume]
        releaseMultipliers = reverse $ take (length pureFreqSound) $ map (min 1.0) [0.0, release ..]

note :: Semitones -> Seconds -> Float -> Float -> Sound
note duration attack release n = freqSound duration attack release $ noteFreq n

combineSounds :: [Sound] -> Sound
combineSounds = map sum . transpose

chord :: Seconds -> [Semitones] -> Chord
chord duration ns = combineSounds [note duration 0.0001 0.001 n | n <- ns]

chords :: Sound
chords = concat $ take 16 $ repeat $ concat $ [ chord 1.500 [-7,-4,0,3]
                                              , chord 1.125 [-2,2,-7,-4]
                                              , chord 2.625 [-9,-5,-2,2]
                                              , chord 0.750 [-9,-5,-2,2]
                                              ]

melody :: Sound
melody = concat $ take 4 $ repeat $ concat [ note 0.3750 0.001 0.001 17
                                           , note 0.3750 0.001 0.001 20
                                           , note 0.3750 0.001 0.001 24
                                           , note 0.3750 0.001 0.001 27
                                           , note 0.3750 0.001 0.001 10
                                           , note 0.3750 0.001 0.001 22
                                           , note 0.3750 0.001 0.001 17
                                           , note 2.6250 0.001 0.001 14
                                           , note 0.7500 0.001 0.001 12

                                           , note 0.3750 0.001 0.001 17
                                           , note 0.3750 0.001 0.001 20
                                           , note 0.3750 0.001 0.001 24
                                           , note 0.3750 0.001 0.001 27
                                           , note 0.3750 0.001 0.001 10
                                           , note 0.3750 0.001 0.001 22
                                           , note 0.3750 0.001 0.001 17
                                           , note 2.6250 0.001 0.001 12
                                           , note 0.7500 0.001 0.001 10

                                           , note 0.3750 0.001 0.001 17
                                           , note 0.3750 0.001 0.001 20
                                           , note 0.3750 0.001 0.001 24
                                           , note 0.7500 0.001 0.001 27
                                           , note 0.3750 0.001 0.001 24
                                           , note 0.7500 0.001 0.001 20
                                           , note 0.3750 0.001 0.001 15 -- somewhere over here it sounds wrong
                                           , note 0.3750 0.001 0.001 19
                                           , note 0.3750 0.001 0.001 20
                                           , note 0.9375 0.001 0.001 24
                                           , note 0.1875 0.001 0.001 22
                                           , note 0.7500 0.001 0.001 26

                                           , note 0.3750 0.001 0.001 17
                                           , note 0.3750 0.001 0.001 20
                                           , note 0.3750 0.001 0.001 24
                                           , note 0.3750 0.001 0.001 27
                                           , note 0.7500 0.001 0.001 22
                                           , note 0.7500 0.001 0.001 31
                                           , note 3.0000 0.001 0.001 27
                                           ]

song :: Sound
song = combineSounds [chords, melody]

save :: FilePath -> IO ()
save filePath = B.writeFile filePath $ B.toLazyByteString $ fold $ map B.floatLE song

play :: IO ()
play = do
    save outputFilePath
    _ <- runCommand $ printf "ffplay -showmode 1 -f f32le -ar %f %s" sampleRate outputFilePath
    return ()
