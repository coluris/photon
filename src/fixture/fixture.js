import crypto from "crypto";

export class Fixture {
  #id = "";
  constructor() {
    this.#id = crypto.randomUUID();
  }
}
