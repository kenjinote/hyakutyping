#!/usr/bin/ruby
# waka_XXXX.txt を生成するためのスクリプト
# CD のリソース指定を行う
# Satofumi KAMIMURA

# \todo 詠み上げ速度を調整するための値を出力する "speed: 90%", "speed: 110%" など


if ARGV.length < 1
  print "usage:\n\t" + __FILE__ + " <track defined file>\n"
  exit
end
resource_file = ARGV[0]


def fillOutput(from, to)
  if from.to_i >= to.to_i
    return
  end

  #print from, '-', to, "\n"
  #return

  for i in from .. to
    print i.to_s + ",0,0\n"
  end
end


# CD 定義ファイルの生成
index = 1
File.open(resource_file) { |io|
  while line = io.gets
    if line =~ /(\d+), (\d+), (\d+), (\d+)/

      next_index = $1.to_i      # 和歌番号
      track_id = $2.to_i        # トラック番号
      start_frame = $3.to_i     # 開始フレーム位置
      play_interval = $4.to_i   # 再生フレーム間隔

      # 指定されたトラックまでのリソースを埋める
      fillOutput(index, track_id - 1)

      # 指定されたトラック自体のリソースを出力
      print track_id.to_s + ',' + start_frame.to_s + ',' + play_interval.to_s + "\n"

      index = track_id + 1
    end
  end
  fillOutput(index, 99)
}
