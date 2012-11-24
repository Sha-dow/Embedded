#       Ohjelma, joka laskee kahden taulukon (oikeammin matriisien,
#       joita taulukot edustavat) v‰lisen SAD:n (sum of absolute differences).
#       SAD lasketaan ottamalla taulukoista samoilla kohdilla olevat lukuarvot,
#       laskemalla niiden et‰isyydet (eli erotuksen itseisarvo) ja laskemalla
#       kaikki saadut et‰isyydet yhteen.
#
#       SAD-algoritmia k‰ytet‰‰n vertailtaessa matriisien keskin‰ist‰
#       samankaltaisuutta, ja siit‰ on apua esimerkiksi esineentunnistuksessa
#       sek‰ kuvan- ja videonpakkauksessa.
#
#       Taulukoiden arvot ovat 8-bittisi‰ etumerkittˆmi‰ lukuja,
#       ja ne on tallennettu muistiin mahdollisimman tiiviisti, eli yhdess‰
#       (nelj‰ll‰ jaollisessa) muistiosoitteessa on 4 arvoa.
#
#       Ohjelma saa argumenttina kahden 16 arvoa sis‰lt‰v‰n taulukon alkuosoitteet,
#       ensimm‰inen rekisteriss‰ $a0 ja toinen rekisteriss‰ $a1.
#
#       Ohjelman C-prototyyppi on seuraavanlainen:
#
#       int calcSAD(short const *template[], short const *compared[]);
#
#       Ohjelma palauttaa lasketun SAD:n rekisteriss‰ $v0.
#
#
#       Esimerkki SAD:n toiminnasta kahdella nelj‰n arvon matriisilla
#       matriisit: [ 11 , 77  ]    [ 121, 89 ]
#                  [ 250, 113 ] ja [ 249, 97 ]
#
#       et‰isyys: [ |11-121| , |77-89|  ]   [ 110, 12 ]
#                 [ |250-249|, |113-97| ] = [  1 , 16 ]
#
#       SAD:      110+12+1+16 = 139
#
###############################################################################
#
# Main program to test calcSAD()
#

    .text
    .globl main
main:
    # Create a stack frame for main program
    subu    $sp $sp,8
    sw      $ra 4($sp)      # return address
    sw      $fp 0($sp)      # old frame pointer
    addu    $fp $sp,4       # update frame pointer

    # Get arguments and call the find_one
    la      $a0 test1_1
    la      $a1 test1_2
    jal     calcsad

    .data
test1_1:        .word   0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef
test1_2:        .word   0x5eadbeef, 0x5eadbeef, 0x5eadbeef, 0x5eadbeef
    .text

    # Check result
    li          $t0, 512
    bne         $v0, $t0, .failure1

    # Tell user that find_one works..
    .data
.ok:    .asciiz "The calcSAD() passed all tests\n"
    .text
    li      $v0 4
    la      $a0 .ok
    syscall


#
# Exit from test program
#
.exit:
    li      $v0 10
    syscall



    # Test failed
.failure1:
    .data
.fail1: .asciiz " The find_one() failed test 1.\n"
    .text
    li      $v0 4
    la      $a0 .fail1
    syscall
    j       .exit




##############################################################################
#
#       The subroutine calc_sad 
#
#       DO NOT REMOVE OR EDIT THE FOLLOWING LINE!
#       _start_function_calc_sad_

    .text
    .globl calcsad
calcsad:

#a0 ja a1 rekisteriss‰ alkioiden _osoite_

        and $t4, $t4, $zero     #nollataan tulosrekisteri
        and $v0, $v0, $zero     #nollataan tulosrekisteri
        and $t5, $t5, $zero     #nollataan rekisteri
        addi $t5, $a0, 16       #alkuper‰inen osoite talteen looppia varten


looppi: lw $t0, 0($a0)          #lataa 1eka wordi t0:aan
        lw $t1, 0($a1)          #lataa 2eka wordi t1:aan

        and $t2, $t0, 255       #alin alkio a0:lsta
        and $t3, $t1, 255       #alin alkio a1:st‰
        sub $t3, $t2, $t3       #alkioiden erotus, tallennus @t3
        abs $t3, $t3            #itseisarvo
        add $t4, $t4, $t3       #lis‰t‰‰n tulokseen

        srl $t0, $t0, 8         #vanha alkio piiloon
        srl $t1, $t1, 8

        and $t2, $t0, 255       #alin alkio a0:lsta
        and $t3, $t1, 255       #alin alkio a1:st‰
        sub $t3, $t2, $t3       #alkioiden erotus, tallennus @t3
        abs $t3, $t3            #itseisarvo
        add $t4, $t4, $t3       #lis‰t‰‰n tulokseen


        srl $t0, $t0, 8         #vanha alkio piiloon
        srl $t1, $t1, 8

        and $t2, $t0, 255       #alin alkio a0:lsta
        and $t3, $t1, 255       #alin alkio a1:st‰
        sub $t3, $t2, $t3       #alkioiden erotus, tallennus @t3
        abs $t3, $t3            #itseisarvo
        add $t4, $t4, $t3       #lis‰t‰‰n tulokseen

        srl $t0, $t0, 8         #vanha alkio piiloon
        srl $t1, $t1, 8         #vika kerta

        and $t2, $t0, 255       #alin alkio a0:lsta
        and $t3, $t1, 255       #alin alkio a1:st‰
        sub $t3, $t2, $t3       #alkioiden erotus, tallennus @t3
        abs $t3, $t3            #itseisarvo
        add $t4, $t4, $t3       #lis‰t‰‰n tulokseen


        addi $a0, $a0, 4        #pit‰‰ tuhlata yksi muuttuja alkuper‰ist‰ vertailtavaa osoitetta varten
        addi $a1, $a1, 4
        blt $a0, $t5, looppi    #osotetta kasvatetaan 4:ll‰ ja tehd‰‰n sama looppi


        add $v0, $t4, $zero     #tulos ei voi ylivuotaa 16*255
                                #siirret‰‰n tulos $v0 rekisteriin
                                #siirret‰‰n tulos $v0 rekisteriin

        jr      $ra


    .end

