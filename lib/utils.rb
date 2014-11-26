class Utils
  def self.is_remote?(filename)
    %w(http https).include?(filename.split(':').first)
  end
end