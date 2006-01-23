%symbolmap=qw(ㄅ b ㄆ p ㄇ m ㄈ f ㄉ d ㄊ t ㄋ n ㄌ l ㄍ g ㄎ k ㄏ h ㄐ j ㄑ q ㄒ x ㄓ zh ㄔ ch ㄕ sh ㄖ r ㄗ z ㄘ c ㄙ s ㄧ i ㄨ u ㄩ ü ㄚ a ㄛ o ㄜ e ㄝ e ㄞ ai ㄟ ei ㄠ ao ㄡ ou ㄢ an ㄣ en ㄤ ang ㄥ eng ㄦ er ˊ 2 ˇ 3 ˋ 4 ˙ 5);
for $x (sort keys %symbolmap) { 
print $x, " -> ", $symbolmap{$x}, "\n";}
