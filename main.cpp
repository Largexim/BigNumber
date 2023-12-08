#include <iostream>
#include <time.h>
using namespace std;
class BigNumber{
private:
    long base;
    long digits;
    long size;
    bool isNegative;
    long long* number{};
public:
    //Constructors
    BigNumber() : size(1) ,base(10000000), isNegative(false)  {number = new long long[1]; number[0]=0;}
    BigNumber(long s, bool iN) : base(10000000), size(s) , isNegative(iN) {number = new long long[size];}
    BigNumber(const BigNumber& input) : base(10000000), size(input.size), isNegative(input.isNegative) {

        delete [] number;
        digits = input.digits;
        number = new long long[size];
        for (int i=0; i < size; i++){
            number[i] = input.number[i];
        }
    }
    BigNumber(long long input){
        base = 10000000;
        size=0;
        if(input==0) {
            size = 1;
            number = new long long[size];
            number[0]=0;
        }
        for(int i=1; input/i !=0 ; i*=10)
            size++;
        digits =size;
        size = size/8 + 1;

        number = new long long[size];

        if(input<0) {
            isNegative = true;
            input*=-1;
        } else isNegative = false;

        for(int i=0; i<size; i++) {
            number[i] = input % base;
            input/=base;
        }
        //sizeCheck();
    }
    BigNumber(const string& input) {
        if(input[0]=='-'){
            isNegative = true;
            size = 1;
            int stringSize = -1;
            for (int i = 1; input[i]; i++) {
                stringSize++;
                if ((i % 8) == 0)
                    size++;
            }
            digits = stringSize;
            number = new long long[size];
            for (int i = 0; i < size; i++) {
                long long n = 0;
                long long b = 1;
                for (int j = 0; j < 8; j++) {
                    n += (input[stringSize] - 48) * b;
                    b *= 10;
                    stringSize--;
                    if (stringSize < 1)
                        break;
                }
                number[i] = n;
            }
        }
        else{
            isNegative = false;
            size = 1;
            int stringSize = 0;
            for (int i = 1; input[i]; i++) {
                stringSize++;
                if ((i % 8) == 0)
                    size++;
            }
            digits = stringSize+1;
            number = new long long[size];
            for (int i = 0; i < size; i++) {
                long long n = 0;
                long long b = 1;
                for (int j = 0; j < 8; j++) {
                    n += (input[stringSize] - 48) * b;
                    b *= 10;
                    stringSize--;
                    if (stringSize < 0)
                        break;
                }
                number[i] = n;
            }
        }

    }
    ~BigNumber(){
        delete [] number;
        number = nullptr;
    }
    //Binary operators
    BigNumber operator+ (const BigNumber& input)
    {
        if(isNegative==input.isNegative) {
            BigNumber result = sum(input);
            if(isNegative)
                result.isNegative= true;
            return result;
        }
        else if(isNegative&&!input.isNegative) {
            BigNumber result = minus(input);
            if(max(*this,input)== *this && result != 0)
                result.isNegative= true;
            return result;
        }
        else if(!isNegative&&input.isNegative) {
            BigNumber result = minus(input);
            if(max(*this,input)== input && result != 0)
                result.isNegative= true;
            return result;
        }
    }
    BigNumber operator- (const BigNumber& input){
        if(!isNegative&&!input.isNegative){
            BigNumber result = minus(input);
            if(max(*this,input) == input && result != 0) {
                result.isNegative = true;
            }
            return result;
        }
        else if(isNegative&&input.isNegative) {
            BigNumber result = minus(input);
            if(max(*this,input)== *this && result != 0)
                result.isNegative= true;
            return result;
        }
        else if(isNegative&&!input.isNegative) {
            return sum(input);
        }
        else if(!isNegative&&input.isNegative) {
            BigNumber result = sum(input);
            if(max(*this,input)== input && result != 0)
                result.isNegative= true;


            return result;
        }
    }
    BigNumber operator/ (const BigNumber& input){
        if(input.size==1&&input.number[0]==0){
            cout<<"invalid!! : ";
            return BigNumber("0");
        }
        else {
            if (isNegative == input.isNegative)
                return division(input);
            else {
                BigNumber result = division(input);
                result.isNegative = true;
                return result;
            }
        }
    }
    BigNumber operator* (const BigNumber& input){
        if(isNegative==input.isNegative)
            return multiplication(input);
        else{
            BigNumber result = multiplication(input);
            result.isNegative = true;
            return result;
        }
    }
    BigNumber operator^ (const BigNumber& input){
        if(input.size==1&&input.number[0]==0){
            BigNumber res("1");
            return res;
        }
        else if(input.isNegative){
            BigNumber res("0");
            return res;
        }
        else {
            return power(input);
        }
    }
    BigNumber& operator= (const BigNumber& input){
        digits = input.digits;
        if(this == &input)
            delete [] number;
        size = input.size;
        isNegative = input.isNegative;
        number = new long long[size];
        for (int i=0; i < size; i++){
            number[i] = input.number[i];
        }
        return *this;
    }

    //Comparison operators
    bool operator== (const BigNumber& input){
        if(size!=input.size || isNegative!=input.isNegative)
            return false;
        for(int i=0; i<size; i++){
            if(number[i]!=input.number[i])
                return false;
        }
        return true;
    }
    bool operator!= (const BigNumber& input){
        if(size!=input.size || isNegative!=input.isNegative)
            return true;
        for(int i=0; i<size; i++){
            if(number[i]!=input.number[i])
                return true;
        }
        return false;
    }
    bool operator> (const BigNumber& input){
        if(isNegative&&input.isNegative)
            return smaller(input);
        else if(isNegative&&!input.isNegative)
            return false;
        else if(!isNegative&&input.isNegative)
            return true;
        else
            return bigger(input);
    }
    bool operator>= (const BigNumber& input){
        if(isNegative&&input.isNegative)
            return smallerOrEqual(input);
        else if(isNegative&&!input.isNegative)
            return false;
        else if(!isNegative&&input.isNegative)
            return true;
        else
            return biggerOrEqual(input);
    }
    bool operator< (const BigNumber& input){
        if(isNegative&&input.isNegative)
            return bigger(input);
        else if(isNegative&&!input.isNegative)
            return true;
        else if(!isNegative&&input.isNegative)
            return false;
        else
            return smaller(input);
    }
    bool operator<= (const BigNumber& input){
        if(isNegative&&input.isNegative)
            return biggerOrEqual(input);
        else if(isNegative&&!input.isNegative)
            return true;
        else if(!isNegative&&input.isNegative)
            return false;
        else
            return smallerOrEqual(input);
    }
    void operator++ () {*this = *this + 1;}
    void operator-- () {*this = *this - 1;}
    BigNumber operator% (const BigNumber& input){
            BigNumber result("0");
            result = *this - (*this / input) * input;
            return result;
    }

    //Print
    void print() {
        if (isNegative)
            cout << "-";
        if (size == 1 && number[0] == 0)
            cout << "0";
        else {
            for (long i = size - 1; i >= 0; i--) {
                if (number[i] == 0)
                    cout << "00000000";
                else
                    cout << number[i];
            }
        }
        cout << endl;
    }
    BigNumber squareRoot(){
        BigNumber result;
        BigNumber bR("1");
        BigNumber sR("1");
        for(long i=0; i<=digits/2; i++)
            bR = bR*10;
        if (*this - (bR * bR)>0 && *this - (bR * bR) < bR)
            return bR;
        else if(*this - (sR * sR)>0 && *this - (sR * sR) < sR )
            return sR;
        else {
            result = ((bR+sR)/2);
            while(true){
                if(bR==result)
                    return result;
                if(sR==result)
                    return result;
                if(result*result>*this) {
                    bR = result;
                    result = ((bR+sR)/2);
                }
                else if(*this - (result*result)>0 && *this - (result*result) > result ) {
                    sR = result;
                    result = ((bR+sR)/2);
                }
                else if(*this - (result*result)>=0 && *this - (result*result) < result )
                    return result;
            }
        }
    }

private:

    BigNumber power(const BigNumber& input){
        BigNumber result(*this);
        BigNumber powerTo(input);
        BigNumber carry("1");
            while(powerTo>1){
                if((powerTo%2)!=0)
                    carry = carry * result;
                powerTo = (powerTo-(powerTo%2)) / 2;
                result = result * result;
            }
            result = result * carry;
        return result;
    }
    BigNumber division(BigNumber input){
        BigNumber result;
        if(*this<input) {
            return result = 0;
        }
        BigNumber rb = div(shiftRight(input.digits-1),input.shiftRight(input.digits-1));
        BigNumber rs = div(shiftRight(input.digits-1),input.shiftRight(input.digits-1)+1);
        result = div((rb+rs),2);
        if((rb*input)<=*this){
            if (*this - (rb * input) < input) {
                return rb;
            }
        }
        else {
            while (true) {
                if ((result * input) > *this)
                    rb = result;
                else {
                    if ((*this - (result * input) < input))
                        return result;
                    else
                        rs = result;
                }
                result = div((rb + rs), 2);
            }
        }
    }
    BigNumber div(const BigNumber& n1,const BigNumber& n2) {
        base = 100000000;
        BigNumber result(n1.size, false);
        long long carry = 0;
            for (long j = n1.size-1; j >= 0 ; j--) {
                result.number[j] = ((n1.number[j]+carry)-((n1.number[j]+carry)%n2.number[0])) / (n2.number[0]);
                carry = (((n1.number[j]+carry) % (n2.number[0]))) * base;
            }
            result.sizeCheck();
        return result;
    }
    BigNumber multiplication(const BigNumber& input){
        base = 100000000;
        long long carry = 0;
        BigNumber newNum;
        if(size>=input.size) {
            newNum.size = (size + 1) * 2;
            newNum.number =new long long[newNum.size];
        }
        else {
            newNum.size = (input.size + 1) * 2;
            newNum.number =new long long[newNum.size];
        }

        for(int i=0 ; i<input.size ; i++)
        {
            for(int j=0 ; j<size ; j++)
            {
                if(i==0){
                    long  long x = (input.number[i] * number[j] + carry);
                    newNum.number[j+i] = x % base;
                    carry = (x-(x%base)) / base;

                    if(j == size -1) {
                        newNum.number[j+1+i] = carry;
                        carry = 0;
                    }
                }
                else {
                    long long x = newNum.number[j + i] + (input.number[i] * number[j] + carry);
                    newNum.number[j + i] = x % base;
                    carry = (x-(x%base)) / base;

                    if (j == size - 1) {
                        newNum.number[j + 1 + i] = carry;
                        carry = 0;
                    }
                }
            }
        }
        newNum.sizeCheck();
        return newNum;
    }
    BigNumber minus(const BigNumber& input){
        base = 100000000;
        BigNumber maxNum = max(*this,input);
        BigNumber minNum = min(*this,input);
        BigNumber result(maxNum.size, false);
        long long carry=0;
        for(int i=0; i<result.size; i++){
            if(i>=minNum.size) {
                result.number[i] = (maxNum.number[i] + 10 + carry) % 10;
                if((maxNum.number[i] + base + carry)>=base)
                carry = 0;
                else
                    carry = -1;

            }
            else if(maxNum.number[i]<minNum.number[i]){
                result.number[i] = maxNum.number[i] - minNum.number[i] + base + carry;
                carry = -1;
            }
            else{
                result.number[i] = maxNum.number[i] - minNum.number[i] + carry;
                carry = 0;
            }
        }
        result.sizeCheck();
        return result;
    }
    BigNumber sum(const BigNumber& input){
        base = 100000000;
        long minSize;
        long maxSize;
        if(input.size>=size) {
            minSize = size;
            maxSize = input.size;
        }
        else{
            minSize = input.size;
            maxSize = size;
        }
        BigNumber result(maxSize+1,false);
        long long carry = 0;
        for(int i=0; i<result.size; i++){
            if(i>=minSize){
                if(maxSize==size) {
                    result.number[i] = number[i] + carry;
                    carry = 0;
                }
                else {
                    result.number[i] = input.number[i] + carry;
                    carry = 0;
                }
            }
            else {
                long long sumOf = number[i] + input.number[i] + carry;
                result.number[i] = sumOf % base;
                carry = (sumOf-(sumOf% base)) / base;
            }
        }
        result.sizeCheck();
        return result;
    }

    //Shift functions
    BigNumber shiftLeft(long long n)
    {
        BigNumber newNum;
        newNum = *this;
        for(int i=0 ; i<n;i++){
            newNum = newNum*10;
        }
        return newNum;
    }
    BigNumber shiftRight(long long n){
        string s;
        n++;
        for(int i=0; i<size; i++){
            if(number[2-i-1]==0)
                s+="0000000";
            s += to_string(number[size-i-1]);
        }
        for(int i=0 ; i<=s.length() ;i++){
            if(s.length()-i<n)
                s[s.length()-i] = '0';
            else
                s[s.length()-i]=s[s.length()-(i+n)];
        }
        BigNumber result(s);
        result.sizeCheck();
        return result;
    }

    //Comparison functions
    bool bigger(const BigNumber& input){
        if (size < input.size)
            return false;
        else if (size > input.size)
            return true;
        else {
            for (int i = size - 1; i >= 0; i--) {
                if (number[i] > input.number[i])
                    return true;
                else if (number[i] < input.number[i])
                    return false;
            }
        }
        return false;
    }
    bool biggerOrEqual(const BigNumber& input){
        if (size < input.size)
            return false;
        else if (size > input.size)
            return true;
        else {
            for (int i = size - 1; i >= 0; i--) {
                if (number[i] < input.number[i])
                    return false;
                else if (number[i] > input.number[i])
                    return true;
            }
        }
        return true;
    }
    bool smaller(const BigNumber& input){
        if (size > input.size)
            return false;
        else if (size < input.size)
            return true;
        else {
            for (int i = size - 1; i >= 0; i--) {
                if (number[i] < input.number[i])
                    return true;
                else if (number[i] > input.number[i])
                    return false;
            }
        }
        return false;
    }
    bool smallerOrEqual(const BigNumber& input){
        if(size>input.size)
            return false;
        if(size<input.size)
            return true;
        else{
            for(int i=size-1; i>=0; i--){
                if(number[i]>input.number[i])
                    return false;
                else if(number[i]<input.number[i])
                    return true;
            }
        }
        return true;
    }
    [[nodiscard]] BigNumber max(const BigNumber& n1, const BigNumber& n2) const{
        if(n1.size>n2.size)
            return n1;
        else if(n1.size<n2.size)
            return n2;
        else{
            for(long i=size-1; i>=0; i--){
                if(n1.number[i]>n2.number[i])
                    return n1;
                else if(n1.number[i]<n2.number[i])
                    return n2;
            }
        }
        return n1;
    }
    [[nodiscard]] BigNumber min(const BigNumber& n1, const BigNumber& n2) const{
        if(n1.size<n2.size)
            return n1;
        else if(n1.size>n2.size)
            return n2;
        else{
            for(long i=size-1; i>=0; i--){
                if(n1.number[i]<n2.number[i])
                    return n1;
                else
                    return n2;
            }
        }
        return n1;
    }

    //Function to remove extra zeros
    void sizeCheck()
    {
        if(size==1&&number[0]==0)
            return;
        for(long i=size-1; i>0; i--)
        {
            if(number[i]!=0)
                break;
            size-=1;
        }
        BigNumber result(size,isNegative);
        for(int i=0; i<size ; i++){
            result.number[i]=number[i];
        }
        *this = result;
    }

};
BigNumber enterNumber();
void printMenu();
int main() {
    cout << "         WellCome\nPress ENTER button to start";
    while (cin.get() != '\n');
    BigNumber n1;
    BigNumber n2;
    int menu = 0;
    while (menu != -1) {
            printMenu();
            cin >> menu;
        switch (menu) {
            case 1:{ n1 = enterNumber(); n2 = enterNumber();cout<<"\nResult :";
                clock_t tStart = clock();
                (n1+n2).print();
                printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);
                break;}
            case 2:{ n1 = enterNumber(); n2 = enterNumber();cout<<"\nResult :";
                clock_t tStart = clock();
                (n1-n2).print();
                printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);
                break;}
            case 3:{ n1 = enterNumber(); n2 = enterNumber();cout<<"\nResult :";
                clock_t tStart = clock();
                (n1*n2).print();
                printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);
                break;}
            case 4:{ n1 = enterNumber(); n2 = enterNumber();cout<<"\nResult :";
                clock_t tStart = clock();
                (n1/n2).print();
                printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);
                break;}
            case 5:{ n1 = enterNumber(); n2 = enterNumber();cout<<"\nResult :";
                clock_t tStart = clock();
                (n1^n2).print();
                printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);
                break;}
            case 6: {n1 = enterNumber();cout<<"\nResult :";
                clock_t tStart = clock();
                n1.squareRoot().print();
                printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);
                break;}
            case 7 :{ menu = -1;
                break;}
        }
    }
       return 0;
}

BigNumber enterNumber()
{
    string s;
    while(true) {
        bool error = false;
        cout << "Enter the number : ";
        cin >> s;
        for(int i=0; s[i]; i++){
            if((s[i]<'0'||s[i]>'9')&&s[i]!='-') {
                cout << "invalid number! try again : \n";
                cout<<i;
                error = true;
                break;
            }
        }
        if(!error)
            break;
    }
    BigNumber number(s);
    return number;
}
void printMenu(){
    cout<<endl;
    cout<<"1) NumberOne + NumberTwo."<<"          "<<"2)NumberOne - NumberTwo.\n";
    cout<<"3) NumberOne * NumberTwo."<<"          "<<"4)NumberOne / NumberTwo.\n";
    cout<<"5) NumberOne ^ NumberTwo."<<"          "<<"6)squareRoot.\n";
    cout<<"7)Exit"<<endl;
}
