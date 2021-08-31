#!/usr/bin/ruby
# waka_XXXX.txt �𐶐����邽�߂̃X�N���v�g
# CD �̃��\�[�X�w����s��
# Satofumi KAMIMURA

# \todo �r�ݏグ���x�𒲐����邽�߂̒l���o�͂��� "speed: 90%", "speed: 110%" �Ȃ�


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


# CD ��`�t�@�C���̐���
index = 1
File.open(resource_file) { |io|
  while line = io.gets
    if line =~ /(\d+), (\d+), (\d+), (\d+)/

      next_index = $1.to_i      # �a�̔ԍ�
      track_id = $2.to_i        # �g���b�N�ԍ�
      start_frame = $3.to_i     # �J�n�t���[���ʒu
      play_interval = $4.to_i   # �Đ��t���[���Ԋu

      # �w�肳�ꂽ�g���b�N�܂ł̃��\�[�X�𖄂߂�
      fillOutput(index, track_id - 1)

      # �w�肳�ꂽ�g���b�N���̂̃��\�[�X���o��
      print track_id.to_s + ',' + start_frame.to_s + ',' + play_interval.to_s + "\n"

      index = track_id + 1
    end
  end
  fillOutput(index, 99)
}
