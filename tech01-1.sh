#!/bin/sh

IFS=','

declare -A matrix

e=0.0000001
l=0

  while read line
  do
    k=0
  for i in $line
  do
    matrix[$l,$k]=$i
    ((k++))
  done
  ((l++))
done < $1

for ((j=0;j<l;j++)) do
  if (( $(echo "${matrix[$j,$j]} < $e && ${matrix[$j,$j]} > -$e" | bc -l) ))
  then
    for ((r=j+1;r<l;r++)) do
      if (( $(echo "${matrix[$r,$j]} > $e || ${matrix[$r,$j]} < -$e" | bc -l) ))
      then
        for ((t=j;t<=l;t++)) do
          matrix[$j,$t]=`echo "${matrix[$j,$t]} + ${matrix[$r,$t]}" | bc -l`
        done
        break
      fi
    done
  fi

  val=${matrix[$j,$j]}

  for ((i=0;i<=$l;i++)) do
    matrix[$j,$i]=`echo "${matrix[$j,$i]} / $val" | bc -l`
  done

  for ((z=j+1;z<l;z++)) do
    c=${matrix[$z,$j]}
    for ((w=j;w<=$l;w++)) do
      matrix[$z,$w]=`echo "${matrix[$z,$w]} - $c * ${matrix[$j,$w]}" | bc -l`
    done
  done
done

for ((i=l-1;i>0;i--)) do
  for ((j=i-1;j>=0;j--)) do
    matrix[$j,$l]=`echo "${matrix[$j,$l]} - ${matrix[$i,$l]} * ${matrix[$j,$i]}" | bc -l`
  done
done

for ((i=0;i<$l;i++)) do
  echo ${matrix[$i,$l]}
done

unset matrix