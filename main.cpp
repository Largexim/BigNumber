#include <iostream>
using namespace std;
class BigNumber{
private:
    long size;
    bool isNegative;
    int* number{};
public:
    //Constructors
    BigNumber() : size(1) , isNegative(false)  {number = new int[1]; number[0]=0;}
    BigNumber(long s, bool iN) : size(s) , isNegative(iN) {number = new int[size];}
    BigNumber(const BigNumber& input) : size(input.size), isNegative(input.isNegative) {

        delete [] number;

        number = new int[size];
        for (int i=0; i < size; i++){
            number[i] = input.number[i];
        }
    }
    BigNumber(int input){
        size=0;
        if(input==0) {
            size = 1;
            number = new int[size];
            number[0]=0;
        }
        for(int i=1; input/i !=0 ; i*=10)
            size++;

        number = new int[size];

        if(input<0) {
            isNegative = true;
            input*=-1;
        } else isNegative = false;

        for(int i=0; i<size; i++) {
            number[i] = input % 10;
            input/=10;
        }
        sizeCheck();
    }
    BigNumber(const string& input) {

        if(input[0] == '-') {
            isNegative = true;

            size = -1;
            for (int i =0; input[i]; i++) {
                size++;
            }

            number = new int[size];
            for (int i = 0; i < size+1; i++) {
                number[i] = input[size  - i] - 48;
            }
        }
        else
        {
            isNegative = false;
            size = 0;
            for (int i =0; input[i]; i++) {
                size++;
            }

            number = new int[size];
            for (int i = 0; i < size; i++) {
                number[i] = input[size - 1 - i] - 48;
            }
        }
        sizeCheck();
    }
    ~BigNumber(){
        delete [] number;
        number = nullptr;
    }

    //Binary operators
    BigNumber operator+ (const BigNumber& input)
    {
        if(isNegative==input.isNegative) {
            return sum(input);
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
        if(this == &input)
            delete [] number;
        size = input.size;
        isNegative = input.isNegative;
        number = new int[size];
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

    //Print
    void print(){
        if(isNegative)
            cout<<"-";
        for(long i=size-1; i>=0; i--)
            cout<<number[i];
        cout<<endl;
    }

private:
    //Calculator functions
    BigNumber power(const BigNumber& input){
        BigNumber newNum("1");
        BigNumber pow = input;
        while(true)
        {
            newNum = newNum * *this;
            pow = pow - 1;
            if(pow == 0) {
                return newNum;;
            }
        }
    }
    BigNumber division(const BigNumber& input){
        BigNumber result;
        if(*this<input) {
            return result = 0;
        }
        BigNumber rb = div(shiftRight(input.size-1),input.number[input.size-1]);
        BigNumber rs = div(shiftRight(input.size-1),input.number[input.size-1]+1);
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
    BigNumber div(const BigNumber& n1,const int& n2) {
        BigNumber result(n1.size, false);
        int carry = 0;
            for (long j = n1.size-1; j >= 0 ; j--) {
                result.number[j] = (n1.number[j]+carry) / (n2);
                carry = (((n1.number[j]+carry) % (n2))) * 10;
            }
            result.sizeCheck();
        return result;
    }
    BigNumber multiplication(const BigNumber& input){
        int carry = 0;
        BigNumber maxNum = max(*this,input);
        BigNumber minNum = min(*this,input);
        BigNumber newNum;
        if(maxNum==input) {
            newNum.size = (input.size + 1) * 2;
            newNum.number = new int[(input.size + 1) * 2];
            {
                for(int i=0; i<newNum.size; i++){
                    newNum.number[i]=0;
                }
            }
        }
        else {
            newNum.size = (size + 1) * 2;
            newNum.number = new int[(size + 1) * 2];
            for(int i=0; i<newNum.size; i++){
                newNum.number[i]=0;
            }
        }

        for(int i=0 ; i<input.size ; i++)
        {
            for(int j=0 ; j<size ; j++)
            {
                int x = newNum.number[j+i] + (input.number[i] * number[j] + carry);
                newNum.number[j+i] = x % 10;
                carry = x  / 10;

                if(j == size -1) {
                    newNum.number[j+1+i] = carry;
                    carry = 0;
                }
            }
        }
        newNum.sizeCheck();
        return newNum;
    }
    BigNumber minus(const BigNumber& input){
        BigNumber maxNum = max(*this,input);
        BigNumber minNum = min(*this,input);
        BigNumber result(maxNum.size, false);
        int carry=0;
        for(int i=0; i<result.size; i++){
            if(i>=minNum.size) {
                result.number[i] = (maxNum.number[i] + 10 + carry) % 10;
                if((maxNum.number[i] + 10 + carry)>=10)
                carry = 0;
                else
                    carry = -1;

            }
            else if(maxNum.number[i]<minNum.number[i]){
                result.number[i] = maxNum.number[i] - minNum.number[i] + 10 + carry;
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
        long minSize = min(*this,input).size;
        long maxSize = max(*this,input).size;
        BigNumber result(maxSize+1,isNegative&&input.isNegative);
        int carry = 0;
        for(int i=0; i<result.size; i++){
            if(i>=minSize){
                result.number[i] = max(*this,input).number[i] + carry;
                carry=0;
            }
            else {
                int sumOf = number[i] + input.number[i] + carry;
                result.number[i] = sumOf % 10;
                carry = sumOf / 10;
            }
        }
        result.sizeCheck();
        return result;
    }

    //Shift functions
    BigNumber shiftLeft(int n)
    {
        BigNumber NewNum(size+n,isNegative);
        NewNum.number = new int[NewNum.size];
        for(int i=0; i<n ; i++)
        {
            NewNum.number[i] = 0;
        }
        for(int i = n, j = 0 ; i<NewNum.size ; i++, j++ )
        {
            NewNum.number[i] = number[j];
        }
        return NewNum;
    }
    BigNumber shiftRight(int n){
        BigNumber newNum(size-n,isNegative);
        for(int i=0; i<size-n; i++){
            newNum.number[i]= number[i+n];
        }
        return newNum;
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
        BigNumber newNum(size, isNegative);
        for(int i=0; i<size; i++){
            newNum.number[i] = number[i];
        }
        *this = newNum;
    }

};
BigNumber enterNumber();
void printMenu();
int main() {
    cout<<"         WellCome\nPress ENTER button to start\n";
    while (cin.get() != '\n');
    int menu=0;
    while (menu!=-1){
        BigNumber n1 = enterNumber();
        BigNumber n2 = enterNumber();
        printMenu();
        while(menu!=-1) {
            while (true) {
                cin >> menu;
                if (menu < 1 || menu > 8) {
                    cout << "enter valid number : ";
                } else break;
            }
            switch (menu) {
                case 1:
                    cout<<"\n";
                    cout<<"Result : ";
                    (n1 + n2).print();
                    printMenu();
                    break;
                case 2:
                    cout<<"\n";
                    cout<<"Result : ";
                    (n1 - n2).print();
                    printMenu();
                    break;
                case 3:
                    cout<<"\n";
                    cout<<"Result : ";
                    (n1 * n2).print();
                    printMenu();
                    break;
                case 4:
                    cout<<"\n";
                    cout<<"Result : ";
                    (n1 / n2).print();
                    printMenu();
                    break;
                case 5:
                    cout<<"\n";
                    cout<<"Result : ";
                    (n1 ^ n2).print();
                    printMenu();
                    break;
                case 6:
                    n1 = enterNumber();
                    printMenu();
                    break;
                case 7:
                    n2 = enterNumber();
                    printMenu();
                    break;
                case 8:
                    menu = -1;
            }
        }
    }
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
    cout<<"5) NumberOne ^ NumberTwo."<<"          "<<"6)Change numberOne.\n";
    cout<<"7)Change numberTwo"<<"          "<<"8)Exit.\n";
}
