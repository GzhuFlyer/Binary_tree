#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"include/tree.h"

char menu(void);
void addpet(Tree * pt);
void droppet(Tree * pt);
void showpets(const Tree * pt);
void findpet(const Tree * pt);
void printitem(Item item);
void uppercase(char * str);

int main()
{
    Tree pets;
    char choice;
    InitializeTree(&pets);  //初始化，将结构体里面的指针指向NULL,成员数初始化为0.
    while((choice = menu()) != 'q') //调用menu函数，打印提示信息。
    {
        printf("------------>%p\n",pets);
        printf("root-------->%p\n",pets.root);
        switch(choice)
        {
            case 'a' : addpet(&pets);
                break;
            case 'l' : showpets(&pets);
                break;
            case 'f': findpet(&pets);
                break;
            case 'n': printf("%d pets in club\n",
                    TreeItemCount(&pets));
                break;
            case 'd': droppet(&pets);
                break;
            default: puts("Switching error");
        }
    }
        DeleteAll(&pets);
        puts("Bye.");
        return 0;
}

char menu(void)
{
    int ch;
    puts("Nerfville Pet Club Membership Program");
    puts("Enter the letter corresponding to your choice:");
    puts("a)add a pet           l)show list of pets");
    puts("n)number of pets      f)find pets");
    puts("d)delete a pet         q)quit");
    /**在下面的getchar函数中，由于控制台一开始输入缓冲区没有可以接受的数据，
     * 控制台会处于阻塞状态，一直接受输入的字符，并存放到缓冲区中，
     * 直到输入的字符是换行符，才停止接受字符。然后getchar()函数读取第一个字符赋值
     * 给ch。当下次如果继续调用getchar()函数，则会从上面的缓冲区中读取接下来的字符。
     * 所以在这里的程序中有了该语句        while(getchar() != '\n')continue;
     * 
     **/
    while((ch = getchar()) != EOF)
    {
        //如果上一个getchar()等待输入中，我们只输入了一个换行，则这里会处于阻塞接受字符状态，因为输入缓冲
        //区已经没有什么东西可以读取了。如果上一个getchar()等待输入中,输入了其他字符和换行，则我们在这里
        //不断的读取缓冲区的字符，直到把换行符读取到了，在退出循环。这样就能起到清空输入缓冲区的目的。使用
        //rewind(stdin)同样能清空缓冲区（没试过）。
        while(getchar() != '\n')
            continue;   //continue 的作用是直到上面的条件符合才退出。
        ch = tolower(ch);       //如果输入的是大写字母，转换成小写的。
        if(strchr("alrfndq",ch) == NULL)
            puts("please enter an a, l,f,n,d,or q:");
        else
            break;
    }
    if(ch == EOF)
        ch = 'q';
        
    return ch;
}
void addpet(Tree * pt)
{
    Item temp;
    
    if(TreeIsFull(pt))       //通过结构体成员里面的size变量查看是否达到设定的最大的初始化的值。   
    puts("No room in the club!");
    else
    {
        puts("Please enter name of pet: ");
        fgets(temp.petname,sizeof(temp.petname),stdin);
        puts("Please enter pet kind: ");
        fgets(temp.petkind,sizeof(temp.petkind),stdin);
        uppercase(temp.petname);
        uppercase(temp.petkind);
        AddItem(&temp,pt);
    }
}
void showpets(const Tree * pt)
{
    if(TreeIsEmpty(pt))
        puts("No entries!");
    else
        Traverse(pt,printitem);
}

void printitem(Item item)
{
    printf("Pet::%-19s Kind: %-19s\n",item.petname,item.petkind);
}
void findpet(const Tree * pt)
{
    Item temp;
    if(TreeIsEmpty(pt))
    {
        puts("No entries!");
        return;
    }

    puts("Please enter name of pet you wish to find: ");
    fgets(temp.petname,sizeof(temp.petname),stdin);
    puts("Please enter pet kind: ");
    fgets(temp.petkind,sizeof(temp.petkind),stdin);
    uppercase(temp.petkind);
    uppercase(temp.petname);
    printf("%s the %s ",temp.petname,temp.petkind);
    if(InTree(&temp,pt))
        printf("is a member.\n");
    else
        printf("is not a member.\n");
}

void droppet(Tree * pt)
{
    Item temp;

    if(TreeIsEmpty(pt))
    {
        puts("No entries!");
        return ;
    }
    
    puts("Please enter name of pet you wish to delete: ");
    fgets(temp.petkind,sizeof(temp.petkind),stdin);
    puts("Please enter pet kind: ");
    fgets(temp.petkind,sizeof(temp.petkind),stdin);
    uppercase(temp.petkind);
    uppercase(temp.petname);
    printf("%s the %s ",temp.petname,temp.petkind);
    if(DeleteItem(&temp,pt))
        printf("is dropped from the club.\n");
    else
        printf("is not a member. \n");
}
void uppercase(char * str)
{
    while(*str)
    {
        *str = toupper(*str);
        str++;
    }
}
