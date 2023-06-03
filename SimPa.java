import java.util.*;


public class SimPa {
    public static void main(String[] args){

        Scanner sc = new Scanner(System.in);

        //1.redak -- ucitavanje ulaznih nizova
        String ulazniNizoviString = sc.nextLine();
        List<String> ulazniNizovi = List.of(ulazniNizoviString.split("\\|"));
        List<List<String>> nizovi_nizova = new ArrayList<>();
        for (String s : ulazniNizovi){
            nizovi_nizova.add(List.of(s.split(",")));
        }
        //System.out.println(nizovi_nizova);

        //2.redak -- ucitavanje skupa stanja
        String skupStanjaString = sc.nextLine();
        Set<String> skupStanja = Set.of(skupStanjaString.split(","));

        //3.redak -- ucitavanje skupa ulaznih znakova
        String skupUlaznihZnakovaString = sc.nextLine();
        Set<String> skupUlaznihZnakova = Set.of(skupUlaznihZnakovaString.split(","));

        //4.redak -- ucitavanje skupa znakova stoga
        String skupZnakovaStogaString = sc.nextLine();
        Set<String> skupZnakovaStoga = Set.of(skupZnakovaStogaString.split(","));

        //5.redak -- ucitavanje skupa prihvatljivih stanja
        String skupPrihvatljivihStanjaString = sc.nextLine();
        Set<String> skupPrihvatljivihStanja = Set.of(skupPrihvatljivihStanjaString.split(","));

        //6.redak -- ucitavanje pocetnog stanja
        String pocetnoStanje = sc.nextLine();

        //7.redak -- ucitavanje pocetnog stanja stoga
        String pocetniZnakStoga = sc.nextLine();

        //8.redak i dalje -- ucitavanje funkcija prijelaza
        List<Vector<String>> fje_prijelaza = new ArrayList<>();
        while(sc.hasNextLine()){

            String funkcija = sc.nextLine();
            if (funkcija.equals("")){
                break;
            }
            Vector<String> vector = new Vector<>();
            String[] pom1 = funkcija.split("->");
            String[] pom2 = pom1[0].split(",");
            String[] pom3 = pom1[1].split(",");
            vector.add(pom2[0]);
            vector.add(pom2[1]);
            vector.add(pom2[2]);
            vector.add(pom3[0]);
            vector.add(pom3[1]);

            //System.out.println(vector);
            //System.out.printf("\n\n");

            fje_prijelaza.add(vector);
        }


        //vanjska petlja po nizovima
        for (List<String> niz : nizovi_nizova){

            //priprema za algoritam
            String trenutnoStanje = pocetnoStanje;
            boolean nijeProcitanZnak = true;
            boolean usoUFail = false;
            Stack<String> stogAutomata = new Stack<>();
            stogAutomata.push(pocetniZnakStoga);

            //printanje pocetnog stanja i stoga
            System.out.printf("%s#%s", trenutnoStanje, stogAutomata.peek());

            //postoji li kombinacija prelaska
            boolean kombinacijaPrelaskaNePostoji = true;

            //unutarnja petlja po elementima pojedinog niza
            for (String ch : niz){

                if (!usoUFail){
                    nijeProcitanZnak = true;
                }

                while(nijeProcitanZnak){

                    nijeProcitanZnak = true;

                    kombinacijaPrelaskaNePostoji = true;

                    //dohvat vrha stoga
                    String uzeoSaStoga;
                    if (!stogAutomata.isEmpty()){
                        uzeoSaStoga = stogAutomata.pop();
                    } else{
                        uzeoSaStoga = "$";
                    }
                    //System.out.printf("  %s  ", uzeoSaStoga);

                    //trazenje kombinacije
                    for (Vector<String> vec : fje_prijelaza){

                        if (vec.get(0).equals(trenutnoStanje) &&
                            vec.get(2).equals(uzeoSaStoga) &&
                                (vec.get(1).equals(ch) || vec.get(1).equals("$"))){

                            //postoji kombinacija za uspjesan prijelaz
                            if (vec.get(1).equals(ch)){
                                nijeProcitanZnak = false;
                            }
                            kombinacijaPrelaskaNePostoji = false;
                            trenutnoStanje = vec.get(3);
                            String zaStavitNaStog = vec.get(4);

                            if(!zaStavitNaStog.equals("$")){

                                //stavljanje na stog
                                for (int i = zaStavitNaStog.length() - 1; i >= 0; i--){
                                    stogAutomata.push(String.valueOf(zaStavitNaStog.charAt(i)));
                                }

                            }

                            //printanje stanja
                            System.out.printf("|%s#", trenutnoStanje);
                            if(stogAutomata.isEmpty()){
                                System.out.printf("$");
                            } else {
                                Stack<String> stogPomocni = new Stack<>();
                                while (!stogAutomata.isEmpty()){
                                    String privremeniSpremnik = stogAutomata.pop();
                                    System.out.printf("%s", privremeniSpremnik);
                                    stogPomocni.push(privremeniSpremnik);
                                }
                                while (!stogPomocni.isEmpty()){
                                    stogAutomata.push(stogPomocni.pop());
                                }
                            }
                            break;
                        } ///zavrsetak ispitivanja pronalaska kombinacije
                    } //zavrsetak petlje vektora

                    if (kombinacijaPrelaskaNePostoji){
                        System.out.printf("|fail|0");
                        usoUFail = true;
                        nijeProcitanZnak = false;
                        break;
                    }
                } //zavrsetak while petlje
            } //zavrsetak petlje za ulazni znak

            //ispitivanje je li automat u prihvatljivom stanju
            if (skupPrihvatljivihStanja.contains(trenutnoStanje) && (!kombinacijaPrelaskaNePostoji)){
                System.out.printf("|1");
            } else if (kombinacijaPrelaskaNePostoji){

            } else{

                boolean nalazimoSeUNeprStanju = true;
                while (!stogAutomata.isEmpty() && nalazimoSeUNeprStanju){

                    String vrhStoga;
                    if (!stogAutomata.isEmpty()){
                        vrhStoga = stogAutomata.pop();
                    } else {
                        vrhStoga = "$";
                    }

                    boolean prijelazPronaden = false;
                    for (Vector<String> v : fje_prijelaza){
                        if(v.get(0).equals(trenutnoStanje) &&
                                v.get(1).equals("$") &&
                                v.get(2).equals(vrhStoga)){

                            prijelazPronaden = true;
                            trenutnoStanje = v.get(3);
                            String naStog = v.get(4);

                            //za stavit na stog
                            if (!naStog.equals("$")){
                                for (int i = naStog.length() - 1; i >= 0; i--){
                                    stogAutomata.push(String.valueOf(naStog.charAt(i)));
                                }
                            }

                            //printanje
                            System.out.printf("|%s#", trenutnoStanje);

                            if(stogAutomata.isEmpty()){
                                System.out.printf("$");
                            } else {
                                Stack<String> stogPomocni = new Stack<>();
                                while (!stogAutomata.isEmpty()){
                                    String privremeniSpremnik = stogAutomata.pop();
                                    System.out.printf("%s", privremeniSpremnik);
                                    stogPomocni.push(privremeniSpremnik);
                                }
                                while (!stogPomocni.isEmpty()){
                                    stogAutomata.push(stogPomocni.pop());
                                }
                            }

                            if (skupPrihvatljivihStanja.contains(trenutnoStanje)) {
                                nalazimoSeUNeprStanju = false;
                                break;
                            }
                        }
                    }
                    if (!prijelazPronaden){
                        break;
                    }

                }


                if (skupPrihvatljivihStanja.contains(trenutnoStanje)){
                    System.out.printf("|1");
                } else{
                    System.out.printf("|0");
                }

            }

            System.out.println("");
        }


    }
}
